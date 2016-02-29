#include <stdlib.h>

#include "rbtree.h"
#include "connect.h"

struct conn_type *conn_search(struct rb_root *root, struct conn_node *conn)
{
    struct rb_node *node = root->rb_node;
    while (node) {
        struct conn_type *data = container_of(node, struct conn_type, my_node);

        if (conn->conn_fd  <  data->node->conn_fd)
            node = node->rb_left;
        else if (conn->conn_fd  >  data->node->conn_fd)
            node = node->rb_right;
        else
            return data;
    }
    return NULL;
}

int conn_insert(struct rb_root *root, struct conn_type *data)
{
    struct rb_node **tmp = &(root->rb_node), *parent = NULL;

    /* Figure out where to put new node */
    while (*tmp) {

        struct conn_type *this = container_of(*tmp, struct conn_type, my_node);

        parent = *tmp;
        if (data->node->conn_fd  < this->node->conn_fd)
             tmp = &((*tmp)->rb_left);
        else if (data->node->conn_fd  > this->node->conn_fd)
             tmp = &((*tmp)->rb_right);
        else if (data->node->conn_fd == this->node->conn_fd)
        {
             //覆盖原有数据
             this->node->clientaddr=data->node->clientaddr;
             this->node->conn_fd=data->node->conn_fd;
             this->node->epoll_fd=data->node->epoll_fd;

             free(data->node);
             free(data);
             return 0;
        }
        else
            return -1;
    }

    /* Add new node and rebalance tree. */
    rb_link_node(&data->my_node, parent, tmp);
    rb_insert_color(&data->my_node, root);

    return 0;
}


void conn_delete(struct rb_root *root, struct conn_node *conn)
{
    struct conn_type *data = conn_search(root, conn);
    if (!data) {
        fprintf(stderr, "Not found %d.\n", conn->conn_fd);
        return;
    }

    rb_erase(&data->my_node, root);
    free(data->node); //free
    free(data);     //free
}


void print_rbtree(struct rb_root *tree)
{
    struct rb_node *node;

    for (node = rb_first(tree); node; node = rb_next(node))
        printf("%d  ", rb_entry(node, struct conn_type, my_node)->node->conn_fd);

    printf("\n");
}
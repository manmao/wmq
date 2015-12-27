#include <stdlib.h>

#include "rbtree.h"
#include "connect.h"


struct conn_type *conn_search(struct rb_root *root, struct conn_node *conn)
{
    struct rb_node *node = root->rb_node;
    while (node) {
        struct conn_type *data = container_of(node, struct conn_type, my_node);

        if (conn->accept_fd  <  data->node->accept_fd)
            node = node->rb_left;
        else if (conn->accept_fd  >  data->node->accept_fd)
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
        if (data->node->accept_fd  < this->node->accept_fd)
             tmp = &((*tmp)->rb_left);
        else if (data->node->accept_fd  > this->node->accept_fd)
             tmp = &((*tmp)->rb_right);
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
        fprintf(stderr, "Not found %d.\n", conn->accept_fd);
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
        printf("%d  ", rb_entry(node, struct conn_type, my_node)->node->accept_fd);

    printf("\n");
}
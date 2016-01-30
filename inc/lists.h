/*
 *  Copyright (C) 2007-2008 tgapsystem
 *      
 *  Author     : zth 
 *  CreateDate : .
 *  Description: 
 *
 *  
 *
 */

 
#ifndef __LISTS_H__
#define __LISTS_H__

#include <pthread.h>


#if defined(SOLARIS) || defined(BSD)
#define TGAP_MUTEX_INIT_W_CONSTRUCTORS
#endif


#if defined(HAVE_PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP) && defined(HAVE_PTHREAD_MUTEX_RECURSIVE_NP)
#define PTHREAD_MUTEX_INIT_VALUE    PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP
#define TGAP_MUTEX_KIND            PTHREAD_MUTEX_RECURSIVE_NP
#else
#define PTHREAD_MUTEX_INIT_VALUE    PTHREAD_MUTEX_INITIALIZER
#define TGAP_MUTEX_KIND            PTHREAD_MUTEX_RECURSIVE
#endif /* PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP */

/*!
  \file tgap_lists.h
  \brief A set of macros to manage forward-linked lists.
*/

/*!
  \brief Locks a list.
  \param head This is a pointer to the list head structure

  This macro attempts to place an exclusive lock in the
  list head structure pointed to by head.
  \retval 0 on success
  \retval non-zero on failure
*/
#define TGAP_LIST_LOCK(head)                        \
    pthread_mutex_lock(&(head)->lock) 

/*!
  \brief Write locks a list.
  \param head This is a pointer to the list head structure

  This macro attempts to place an exclusive write lock in the
  list head structure pointed to by head.
  \retval 0 on success
  \retval non-zero on failure
*/
#define TGAP_RWLIST_WRLOCK(head)                                         \
        pthread_rwlock_wrlock(&(head)->lock)

/*!
  \brief Read locks a list.
  \param head This is a pointer to the list head structure

  This macro attempts to place a read lock in the
  list head structure pointed to by head.
  \retval 0 on success
  \retval non-zero on failure
*/
#define TGAP_RWLIST_RDLOCK(head)                                         \
        pthread_rwlock_rdlock(&(head)->lock)
    
/*!
  \brief Locks a list, without blocking if the list is locked.
  \param head This is a pointer to the list head structure

  This macro attempts to place an exclusive lock in the
  list head structure pointed to by head.
  \retval 0 on success
  \retval non-zero on failure
*/
#define TGAP_LIST_TRYLOCK(head)                        \
    pthread_mutex_trylock(&(head)->lock) 

/*!
  \brief Write locks a list, without blocking if the list is locked.
  \param head This is a pointer to the list head structure

  This macro attempts to place an exclusive write lock in the
  list head structure pointed to by head.
  \retval 0 on success
  \retval non-zero on failure
*/
#define TGAP_RWLIST_TRYWRLOCK(head)                                      \
       pthread_rwlock_trywrlock(&(head)->lock)

/*!
  \brief Read locks a list, without blocking if the list is locked.
  \param head This is a pointer to the list head structure

  This macro attempts to place a read lock in the
  list head structure pointed to by head.
  \retval 0 on success
  \retval non-zero on failure
*/
#define TGAP_RWLIST_TRYRDLOCK(head)                                     \
    pthread_rwlock_tryrdlock(&(head)->lock)
    
/*!
  \brief Attempts to unlock a list.
  \param head This is a pointer to the list head structure

  This macro attempts to remove an exclusive lock from the
  list head structure pointed to by head. If the list
  was not locked by this thread, this macro has no effect.
*/
#define TGAP_LIST_UNLOCK(head)                         \
    pthread_mutex_unlock(&(head)->lock)

/*!
  \brief Attempts to unlock a read/write based list.
  \param head This is a pointer to the list head structure

  This macro attempts to remove a read or write lock from the
  list head structure pointed to by head. If the list
  was not locked by this thread, this macro has no effect.
*/
#define TGAP_RWLIST_UNLOCK(head)                                         \
        pthread_rwlock_unlock(&(head)->lock)

/*!
  \brief Defines a structure to be used to hold a list of specified type.
  \param name This will be the name of the defined structure.
  \param type This is the type of each list entry.

  This macro creates a structure definition that can be used
  to hold a list of the entries of type \a type. It does not actually
  declare (allocate) a structure; to do that, either follow this
  macro with the desired name of the instance you wish to declare,
  or use the specified \a name to declare instances elsewhere.

  Example usage:
  \code
  static TGAP_LIST_HEAD(entry_list, entry) entries;
  \endcode

  This would define \c struct \c entry_list, and declare an instance of it named
  \a entries, all intended to hold a list of type \c struct \c entry.
*/
#define TGAP_LIST_HEAD(name, type)                    \
struct name {                                \
    struct type *first;                        \
    struct type *last;                        \
    pthread_mutex_t lock;                        \
}

/*!
  \brief Defines a structure to be used to hold a read/write list of specified type.
  \param name This will be the name of the defined structure.
  \param type This is the type of each list entry.

  This macro creates a structure definition that can be used
  to hold a list of the entries of type \a type. It does not actually
  declare (allocate) a structure; to do that, either follow this
  macro with the desired name of the instance you wish to declare,
  or use the specified \a name to declare instances elsewhere.

  Example usage:
  \code
  static TGAP_RWLIST_HEAD(entry_list, entry) entries;
  \endcode

  This would define \c struct \c entry_list, and declare an instance of it named
  \a entries, all intended to hold a list of type \c struct \c entry.
*/
#define TGAP_RWLIST_HEAD(name, type)                                     \
struct name {                                                           \
        struct type *first;                                             \
        struct type *last;                                              \
        pthread_rwlock_t lock;                                              \
}

/*!
  \brief Defines a structure to be used to hold a list of specified type (with no lock).
  \param name This will be the name of the defined structure.
  \param type This is the type of each list entry.

  This macro creates a structure definition that can be used
  to hold a list of the entries of type \a type. It does not actually
  declare (allocate) a structure; to do that, either follow this
  macro with the desired name of the instance you wish to declare,
  or use the specified \a name to declare instances elsewhere.

  Example usage:
  \code
  static TGAP_LIST_HEAD_NOLOCK(entry_list, entry) entries;
  \endcode

  This would define \c struct \c entry_list, and declare an instance of it named
  \a entries, all intended to hold a list of type \c struct \c entry.
*/
#define TGAP_LIST_HEAD_NOLOCK(name, type)                \
struct name {                                \
    struct type *first;                        \
    struct type *last;                        \
}

/*!
  \brief Defines initial values for a declaration of TGAP_LIST_HEAD
*/
#define TGAP_LIST_HEAD_INIT_VALUE    {        \
    .first = NULL,                    \
    .last = NULL,                    \
    .lock = PTHREAD_MUTEX_INIT_VALUE,            \
    }

/*!
  \brief Defines initial values for a declaration of TGAP_RWLIST_HEAD
*/
#define TGAP_RWLIST_HEAD_INIT_VALUE      {               \
        .first = NULL,                                  \
        .last = NULL,                                   \
        .lock = PTHREAD_RWLOCK_INITIALIZER,                  \
        }

/*!
  \brief Defines initial values for a declaration of TGAP_LIST_HEAD_NOLOCK
*/
#define TGAP_LIST_HEAD_NOLOCK_INIT_VALUE    {    \
    .first = NULL,                    \
    .last = NULL,                    \
    }

/*!
  \brief Defines a structure to be used to hold a list of specified type, statically initialized.
  \param name This will be the name of the defined structure.
  \param type This is the type of each list entry.

  This macro creates a structure definition that can be used
  to hold a list of the entries of type \a type, and allocates an instance
  of it, initialized to be empty.

  Example usage:
  \code
  static TGAP_LIST_HEAD_STATIC(entry_list, entry);
  \endcode

  This would define \c struct \c entry_list, intended to hold a list of
  type \c struct \c entry.
*/

#define TGAP_LIST_HEAD_STATIC(name, type)                \
struct name {                                \
    struct type *first;                        \
    struct type *last;                        \
    pthread_mutex_t lock;                        \
} name = TGAP_LIST_HEAD_INIT_VALUE


/*!
  \brief Defines a structure to be used to hold a read/write list of specified type, statically initialized.
  \param name This will be the name of the defined structure.
  \param type This is the type of each list entry.

  This macro creates a structure definition that can be used
  to hold a list of the entries of type \a type, and allocates an instance
  of it, initialized to be empty.

  Example usage:
  \code
  static TGAP_RWLIST_HEAD_STATIC(entry_list, entry);
  \endcode

  This would define \c struct \c entry_list, intended to hold a list of
  type \c struct \c entry.
*/
#ifndef TGAP_RWLOCK_INIT_VALUE
#define TGAP_RWLIST_HEAD_STATIC(name, type)                              \
struct name {                                                           \
        struct type *first;                                             \
        struct type *last;                                              \
        pthread_rwlock_t lock;                                              \
} name;                                                                 \
static void  __attribute__ ((constructor)) __init_##name(void)          \
{                                                                       \
        TGAP_RWLIST_HEAD_INIT(&name);                                    \
}                                                                       \
static void  __attribute__ ((destructor)) __fini_##name(void)           \
{                                                                       \
        TGAP_RWLIST_HEAD_DESTROY(&name);                                 \
}                                                                       \
struct __dummy_##name
#else
#define TGAP_RWLIST_HEAD_STATIC(name, type)                              \
struct name {                                                           \
        struct type *first;                                             \
        struct type *last;                                              \
        pthread_rwlock_t lock;                                              \
} name = TGAP_RWLIST_HEAD_INIT_VALUE
#endif

/*!
  \brief Defines a structure to be used to hold a list of specified type, statically initialized.

  This is the same as TGAP_LIST_HEAD_STATIC, except without the lock included.
*/
#define TGAP_LIST_HEAD_NOLOCK_STATIC(name, type)                \
struct name {                                \
    struct type *first;                        \
    struct type *last;                        \
} name = TGAP_LIST_HEAD_NOLOCK_INIT_VALUE

/*!
  \brief Initializes a list head structure with a specified first entry.
  \param head This is a pointer to the list head structure
  \param entry pointer to the list entry that will become the head of the list

  This macro initializes a list head structure by setting the head
  entry to the supplied value and recreating the embedded lock.
*/
#define TGAP_LIST_HEAD_SET(head, entry) do {                \
    (head)->first = (entry);                    \
    (head)->last = (entry);                        \
    pthread_mutex_init(&(head)->lock);                    \
} while (0)

/*!
  \brief Initializes an rwlist head structure with a specified first entry.
  \param head This is a pointer to the list head structure
  \param entry pointer to the list entry that will become the head of the list

  This macro initializes a list head structure by setting the head
  entry to the supplied value and recreating the embedded lock.
*/
#define TGAP_RWLIST_HEAD_SET(head, entry) do {                           \
        (head)->first = (entry);                                        \
        (head)->last = (entry);                                         \
        pthread_rwlock_init(&(head)->lock);                                 \
} while (0)

/*!
  \brief Initializes a list head structure with a specified first entry.
  \param head This is a pointer to the list head structure
  \param entry pointer to the list entry that will become the head of the list

  This macro initializes a list head structure by setting the head
  entry to the supplied value.
*/
#define TGAP_LIST_HEAD_SET_NOLOCK(head, entry) do {            \
    (head)->first = (entry);                    \
    (head)->last = (entry);                        \
} while (0)

/*!
  \brief Declare a forward link structure inside a list entry.
  \param type This is the type of each list entry.

  This macro declares a structure to be used to link list entries together.
  It must be used inside the definition of the structure named in
  \a type, as follows:

  \code
  struct list_entry {
      ...
      TGAP_LIST_ENTRY(list_entry) list;
  }
  \endcode

  The field name \a list here is arbitrary, and can be anything you wish.
*/
#define TGAP_LIST_ENTRY(type)                        \
struct {                                \
    struct type *next;                        \
}

#define TGAP_RWLIST_ENTRY TGAP_LIST_ENTRY
 
/*!
  \brief Returns the first entry contained in a list.
  \param head This is a pointer to the list head structure
 */
#define    TGAP_LIST_FIRST(head)    ((head)->first)

#define TGAP_RWLIST_FIRST TGAP_LIST_FIRST

/*!
  \brief Returns the last entry contained in a list.
  \param head This is a pointer to the list head structure
 */
#define    TGAP_LIST_LTGAP(head)    ((head)->last)

#define TGAP_RWLIST_LTGAP TGAP_LIST_LTGAP

/*!
  \brief Returns the next entry in the list after the given entry.
  \param elm This is a pointer to the current entry.
  \param field This is the name of the field (declared using TGAP_LIST_ENTRY())
  used to link entries of this list together.
*/
#define TGAP_LIST_NEXT(elm, field)    ((elm)->field.next)

#define TGAP_RWLIST_NEXT TGAP_LIST_NEXT

/*!
  \brief Checks whether the specified list contains any entries.
  \param head This is a pointer to the list head structure

  \return non-zero if the list has entries
  \return zero if not.
 */
#define    TGAP_LIST_EMPTY(head)    (TGAP_LIST_FIRST(head) == NULL)

#define TGAP_RWLIST_EMPTY TGAP_LIST_EMPTY

/*!
  \brief Loops over (traverses) the entries in a list.
  \param head This is a pointer to the list head structure
  \param var This is the name of the variable that will hold a pointer to the
  current list entry on each iteration. It must be declared before calling
  this macro.
  \param field This is the name of the field (declared using TGAP_LIST_ENTRY())
  used to link entries of this list together.

  This macro is use to loop over (traverse) the entries in a list. It uses a
  \a for loop, and supplies the enclosed code with a pointer to each list
  entry as it loops. It is typically used as follows:
  \code
  static TGAP_LIST_HEAD(entry_list, list_entry) entries;
  ...
  struct list_entry {
      ...
      TGAP_LIST_ENTRY(list_entry) list;
  }
  ...
  struct list_entry *current;
  ...
  TGAP_LIST_TRAVERSE(&entries, current, list) {
     (do something with current here)
  }
  \endcode
  \warning If you modify the forward-link pointer contained in the \a current entry while
  inside the loop, the behavior will be unpredictable. At a minimum, the following
  macros will modify the forward-link pointer, and should not be used inside
  TGAP_LIST_TRAVERSE() against the entry pointed to by the \a current pointer without
  careful consideration of their consequences:
  \li TGAP_LIST_NEXT() (when used as an lvalue)
  \li TGAP_LIST_INSERT_AFTER()
  \li TGAP_LIST_INSERT_HEAD()
  \li TGAP_LIST_INSERT_TAIL()
*/
#define TGAP_LIST_TRAVERSE(head,var,field)                 \
    for((var) = (head)->first; (var); (var) = (var)->field.next)

#define TGAP_RWLIST_TRAVERSE TGAP_LIST_TRAVERSE

/*!
  \brief Loops safely over (traverses) the entries in a list.
  \param head This is a pointer to the list head structure
  \param var This is the name of the variable that will hold a pointer to the
  current list entry on each iteration. It must be declared before calling
  this macro.
  \param field This is the name of the field (declared using TGAP_LIST_ENTRY())
  used to link entries of this list together.

  This macro is used to safely loop over (traverse) the entries in a list. It
  uses a \a for loop, and supplies the enclosed code with a pointer to each list
  entry as it loops. It is typically used as follows:

  \code
  static TGAP_LIST_HEAD(entry_list, list_entry) entries;
  ...
  struct list_entry {
      ...
      TGAP_LIST_ENTRY(list_entry) list;
  }
  ...
  struct list_entry *current;
  ...
  TGAP_LIST_TRAVERSE_SAFE_BEGIN(&entries, current, list) {
     (do something with current here)
  }
  TGAP_LIST_TRAVERSE_SAFE_END;
  \endcode

  It differs from TGAP_LIST_TRAVERSE() in that the code inside the loop can modify
  (or even free, after calling TGAP_LIST_REMOVE_CURRENT()) the entry pointed to by
  the \a current pointer without affecting the loop traversal.
*/
#define TGAP_LIST_TRAVERSE_SAFE_BEGIN(head, var, field) {                \
    typeof((head)) __list_head = head;                        \
    typeof(__list_head->first) __list_next;                        \
    typeof(__list_head->first) __list_prev = NULL;                    \
    typeof(__list_head->first) __new_prev = NULL;                    \
    for ((var) = __list_head->first, __new_prev = (var),                \
          __list_next = (var) ? (var)->field.next : NULL;                \
         (var);                                    \
         __list_prev = __new_prev, (var) = __list_next,                \
         __new_prev = (var),                            \
         __list_next = (var) ? (var)->field.next : NULL                \
        )

#define TGAP_RWLIST_TRAVERSE_SAFE_BEGIN TGAP_LIST_TRAVERSE_SAFE_BEGIN

/*!
  \brief Removes the \a current entry from a list during a traversal.
  \param field This is the name of the field (declared using TGAP_LIST_ENTRY())
  used to link entries of this list together.

  \note This macro can \b only be used inside an TGAP_LIST_TRAVERSE_SAFE_BEGIN()
  block; it is used to unlink the current entry from the list without affecting
  the list traversal (and without having to re-traverse the list to modify the
  previous entry, if any).
 */
#define TGAP_LIST_REMOVE_CURRENT(field) do { \
    __new_prev->field.next = NULL;                            \
    __new_prev = __list_prev;                            \
    if (__list_prev)                                \
        __list_prev->field.next = __list_next;                    \
    else                                        \
        __list_head->first = __list_next;                    \
    if (!__list_next)                                \
        __list_head->last = __list_prev;                     \
    } while (0)

#define TGAP_RWLIST_REMOVE_CURRENT TGAP_LIST_REMOVE_CURRENT

#define TGAP_LIST_MOVE_CURRENT(newhead, field) do { \
    typeof ((newhead)->first) __list_cur = __new_prev;                \
    TGAP_LIST_REMOVE_CURRENT(field);                            \
    TGAP_LIST_INSERT_TAIL((newhead), __list_cur, field);                \
    } while (0)

#define TGAP_RWLIST_MOVE_CURRENT TGAP_LIST_MOVE_CURRENT

/*!
  \brief Inserts a list entry before the current entry during a traversal.
  \param elm This is a pointer to the entry to be inserted.
  \param field This is the name of the field (declared using TGAP_LIST_ENTRY())
  used to link entries of this list together.

  \note This macro can \b only be used inside an TGAP_LIST_TRAVERSE_SAFE_BEGIN()
  block.
 */
#define TGAP_LIST_INSERT_BEFORE_CURRENT(elm, field) do {            \
    if (__list_prev) {                        \
        (elm)->field.next = __list_prev->field.next;        \
        __list_prev->field.next = elm;                \
    } else {                            \
        (elm)->field.next = __list_head->first;            \
        __list_head->first = (elm);                \
    }                                \
    __new_prev = (elm);                        \
} while (0)

#define TGAP_RWLIST_INSERT_BEFORE_CURRENT TGAP_LIST_INSERT_BEFORE_CURRENT

/*!
  \brief Closes a safe loop traversal block.
 */
#define TGAP_LIST_TRAVERSE_SAFE_END  }

#define TGAP_RWLIST_TRAVERSE_SAFE_END TGAP_LIST_TRAVERSE_SAFE_END

/*!
  \brief Initializes a list head structure.
  \param head This is a pointer to the list head structure

  This macro initializes a list head structure by setting the head
  entry to \a NULL (empty list) and recreating the embedded lock.
*/
#define TGAP_LIST_HEAD_INIT(head) {                    \
    (head)->first = NULL;                        \
    (head)->last = NULL;                        \
    pthread_mutex_init(&(head)->lock, NULL);                    \
}

/*!
  \brief Initializes an rwlist head structure.
  \param head This is a pointer to the list head structure

  This macro initializes a list head structure by setting the head
  entry to \a NULL (empty list) and recreating the embedded lock.
*/
#define TGAP_RWLIST_HEAD_INIT(head) {                                    \
        (head)->first = NULL;                                           \
        (head)->last = NULL;                                            \
        pthread_rwlock_init(&(head)->lock, NULL);                                 \
}

/*!
  \brief Destroys a list head structure.
  \param head This is a pointer to the list head structure

  This macro destroys a list head structure by setting the head
  entry to \a NULL (empty list) and destroying the embedded lock.
  It does not free the structure from memory.
*/
#define TGAP_LIST_HEAD_DESTROY(head) {                    \
    (head)->first = NULL;                        \
    (head)->last = NULL;                        \
    pthread_mutex_destroy(&(head)->lock);                \
}

/*!
  \brief Destroys an rwlist head structure.
  \param head This is a pointer to the list head structure

  This macro destroys a list head structure by setting the head
  entry to \a NULL (empty list) and destroying the embedded lock.
  It does not free the structure from memory.
*/
#define TGAP_RWLIST_HEAD_DESTROY(head) {                                 \
        (head)->first = NULL;                                           \
        (head)->last = NULL;                                            \
        pthread_rwlock_destory(&(head)->lock);                              \
}

/*!
  \brief Initializes a list head structure.
  \param head This is a pointer to the list head structure

  This macro initializes a list head structure by setting the head
  entry to \a NULL (empty list). There is no embedded lock handling
  with this macro.
*/
#define TGAP_LIST_HEAD_INIT_NOLOCK(head) {                \
    (head)->first = NULL;                        \
    (head)->last = NULL;                        \
}

/*!
  \brief Inserts a list entry after a given entry.
  \param head This is a pointer to the list head structure
  \param listelm This is a pointer to the entry after which the new entry should
  be inserted.
  \param elm This is a pointer to the entry to be inserted.
  \param field This is the name of the field (declared using TGAP_LIST_ENTRY())
  used to link entries of this list together.
 */
#define TGAP_LIST_INSERT_AFTER(head, listelm, elm, field) do {        \
    (elm)->field.next = (listelm)->field.next;            \
    (listelm)->field.next = (elm);                    \
    if ((head)->last == (listelm))                    \
        (head)->last = (elm);                    \
} while (0)

#define TGAP_RWLIST_INSERT_AFTER TGAP_LIST_INSERT_AFTER

/*!
  \brief Inserts a list entry at the head of a list.
  \param head This is a pointer to the list head structure
  \param elm This is a pointer to the entry to be inserted.
  \param field This is the name of the field (declared using TGAP_LIST_ENTRY())
  used to link entries of this list together.
 */
#define TGAP_LIST_INSERT_HEAD(head, elm, field) do {            \
        (elm)->field.next = (head)->first;            \
        (head)->first = (elm);                    \
        if (!(head)->last)                    \
            (head)->last = (elm);                \
} while (0)

#define TGAP_RWLIST_INSERT_HEAD TGAP_LIST_INSERT_HEAD

/*!
  \brief Appends a list entry to the tail of a list.
  \param head This is a pointer to the list head structure
  \param elm This is a pointer to the entry to be appended.
  \param field This is the name of the field (declared using TGAP_LIST_ENTRY())
  used to link entries of this list together.

  Note: The link field in the appended entry is \b not modified, so if it is
  actually the head of a list itself, the entire list will be appended
  temporarily (until the next TGAP_LIST_INSERT_TAIL is performed).
 */
#define TGAP_LIST_INSERT_TAIL(head, elm, field) do {            \
      if (!(head)->first) {                        \
        (head)->first = (elm);                    \
        (head)->last = (elm);                    \
      } else {                                \
        (head)->last->field.next = (elm);            \
        (head)->last = (elm);                    \
      }                                    \
} while (0)

#define TGAP_RWLIST_INSERT_TAIL TGAP_LIST_INSERT_TAIL

/*!
  \brief Appends a whole list to the tail of a list.
  \param head This is a pointer to the list head structure
  \param list This is a pointer to the list to be appended.
  \param field This is the name of the field (declared using TGAP_LIST_ENTRY())
  used to link entries of this list together.

  Note: The source list (the \a list parameter) will be empty after
  calling this macro (the list entries are \b moved to the target list).
 */
#define TGAP_LIST_APPEND_LIST(head, list, field) do {            \
      if (!(head)->first) {                        \
        (head)->first = (list)->first;                \
        (head)->last = (list)->last;                \
      } else {                                \
        (head)->last->field.next = (list)->first;        \
        (head)->last = (list)->last;                \
      }                                    \
      (list)->first = NULL;                        \
      (list)->last = NULL;                        \
} while (0)

#define TGAP_RWLIST_APPEND_LIST TGAP_LIST_APPEND_LIST

/*!
  \brief Removes and returns the head entry from a list.
  \param head This is a pointer to the list head structure
  \param field This is the name of the field (declared using TGAP_LIST_ENTRY())
  used to link entries of this list together.

  Removes the head entry from the list, and returns a pointer to it.
  This macro is safe to call on an empty list.
 */
#define TGAP_LIST_REMOVE_HEAD(head, field) ({                \
        typeof((head)->first) cur = (head)->first;        \
        if (cur) {                        \
            (head)->first = cur->field.next;        \
            cur->field.next = NULL;                \
            if ((head)->last == cur)            \
                (head)->last = NULL;            \
        }                            \
        cur;                            \
    })

#define TGAP_RWLIST_REMOVE_HEAD TGAP_LIST_REMOVE_HEAD

/*!
  \brief Removes a specific entry from a list.
  \param head This is a pointer to the list head structure
  \param elm This is a pointer to the entry to be removed.
  \param field This is the name of the field (declared using TGAP_LIST_ENTRY())
  used to link entries of this list together.
  \warning The removed entry is \b not freed nor modified in any way.
 */
#define TGAP_LIST_REMOVE(head, elm, field) ({                    \
    __typeof(elm) __res = NULL; \
    if ((head)->first == (elm)) {                    \
        __res = (head)->first;                      \
        (head)->first = (elm)->field.next;            \
        if ((head)->last == (elm))            \
            (head)->last = NULL;            \
    } else {                                \
        typeof(elm) curelm = (head)->first;            \
        while (curelm && (curelm->field.next != (elm)))            \
            curelm = curelm->field.next;            \
        if (curelm) { \
            __res = (elm); \
            curelm->field.next = (elm)->field.next;            \
            if ((head)->last == (elm))                \
                (head)->last = curelm;                \
        } \
    }                                \
    (elm)->field.next = NULL;                                       \
    (__res); \
})

#define TGAP_RWLIST_REMOVE TGAP_LIST_REMOVE

#endif /* __TGAP_LISTS_H__ */


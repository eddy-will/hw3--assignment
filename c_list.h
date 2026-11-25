/*
 * ============================================================================
 *  c_list.h - a linked list of ints
 * ============================================================================
 *  PROVIDED FILE: do not edit it. The grader uses its own copy, so any
 *  change you make here is ignored and may stop your c_list.c from compiling.
 *
 *  Write the 9 functions declared below in c_list.c, which must begin with:
 *      #include "c_list.h"
 *
 *  RULES FOR EVERY FUNCTION
 *   - `list` and `out` are never NULL.
 *   - list->size must ALWAYS equal the number of nodes in the list.
 *   - Every node is created with malloc and freed with free exactly once.
 *     Removing a node from the list means freeing it.
 *   - Functions that return int return 0 on success and -1 on failure.
 *     On failure, the list (and *out, if there is one) is left EXACTLY
 *     as it was.
 *   - Indexes start at 0, like arrays: index 0 is the head.
 * ============================================================================
 */

#ifndef C_LIST_H
#define C_LIST_H

#include <stddef.h>   /* size_t */

typedef struct Node {
    int value;
    struct Node *next;   /* the next node, or NULL for the last node */
} Node;

typedef struct List {
    Node *head;          /* the first node, or NULL if the list is empty */
    size_t size;         /* the number of nodes */
} List;


/* ----------------------------------------------------------------------------
 *  LEVEL 1: creating, adding, destroying
 * ------------------------------------------------------------------------- */

/*
 * Allocates and returns a new, empty list (head NULL, size 0).
 * Returns NULL if malloc fails.
 */
List *list_create(void);

/*
 * Frees every node in the list, then the list itself.
 * After this call, the caller must not use `list` again.
 */
void list_destroy(List *list);

/*
 * Adds a new node holding `value` at the FRONT of the list.
 * Returns 0, or -1 if malloc fails.
 *
 *   [3 7]  --push_front 1-->  [1 3 7]
 */
int list_push_front(List *list, int value);

/*
 * Adds a new node holding `value` at the END of the list.
 * Returns 0, or -1 if malloc fails.
 *
 *   [3 7]  --push_back 1-->  [3 7 1]
 */
int list_push_back(List *list, int value);


/* ----------------------------------------------------------------------------
 *  LEVEL 2: reading and removing from the front
 * ------------------------------------------------------------------------- */

/*
 * Stores the value at position `index` in *out and returns 0.
 * Returns -1 if index >= size (the list and *out are unchanged).
 * Does not modify the list.
 *
 *   [3 7 5]  get 1  ->  returns 0, *out = 7
 *   [3 7 5]  get 3  ->  returns -1
 */
int list_get(const List *list, size_t index, int *out);

/*
 * Removes the FIRST node, stores its value in *out, frees it, and
 * returns 0. Returns -1 if the list is empty (*out is unchanged).
 *
 *   [3 7 5]  pop_front  ->  returns 0, *out = 3, list is [7 5]
 *   []       pop_front  ->  returns -1
 */
int list_pop_front(List *list, int *out);


/* ----------------------------------------------------------------------------
 *  LEVEL 3: relinking anywhere in the list
 * ------------------------------------------------------------------------- */

/*
 * Inserts a new node holding `value` so that it ends up at position
 * `index`; the nodes from `index` onward move one place back.
 * Valid indexes are 0 .. size (inclusive): index == size adds at the end.
 * Returns 0, or -1 if index > size or malloc fails (list unchanged).
 *
 *   [3 7 5]  insert_at 1, 9  ->  [3 9 7 5]
 *   [3 7 5]  insert_at 0, 9  ->  [9 3 7 5]
 *   [3 7 5]  insert_at 3, 9  ->  [3 7 5 9]
 *   [3 7 5]  insert_at 4, 9  ->  returns -1
 */
int list_insert_at(List *list, size_t index, int value);

/*
 * Removes the node at position `index`, stores its value in *out,
 * frees it, and returns 0.
 * Valid indexes are 0 .. size - 1. Returns -1 if index >= size
 * (the list and *out are unchanged).
 *
 *   [3 7 5]  remove_at 1  ->  returns 0, *out = 7, list is [3 5]
 *   [3 7 5]  remove_at 3  ->  returns -1
 */
int list_remove_at(List *list, size_t index, int *out);

/*
 * Reverses the list IN PLACE by changing the `next` pointers.
 * Do not create or free any nodes, and do not change any node's value:
 * the same nodes must end up in the opposite order.
 *
 *   [3 7 5]  ->  [5 7 3]
 */
void list_reverse(List *list);

#endif /* C_LIST_H */

/*
 * ============================================================================
 *  Assignment: C List (your own linked list library)
 *  Course:     C Programming
 *
 *  Name:       [YOUR NAME]
 *  Student ID: [YOUR ID]
 * ============================================================================
 *
 *  HOW TO BUILD AND RUN
 *      make
 *      ./c_list
 *
 *  `make` compiles this file together with main.c (your test program) using
 *  -Wall -Wextra -Werror. If it builds for you, it builds for the grader.
 *
 *  QUICK RULES (see the README for details)
 *   - Read c_list.h first: its comments are the specification. Don't edit it.
 *   - The only library functions allowed in this file are malloc and free.
 *     No printf here: print from main.c instead.
 *   - Don't change any function's name, return type, or parameters.
 *   - Every node you remove must be freed exactly once. Never use a node
 *     after freeing it: save what you need from it BEFORE calling free.
 *   - list->size must always equal the number of nodes.
 *   - On failure (return -1), change nothing: not the list, not *out.
 *
 *  ABOUT THE "YOUR NOTES" BOXES
 *   Each function has a box for your approach, the edge cases you handled,
 *   and how you tested them. The notes are not graded, but writing them is the
 *   best way to check you understand your own code.
 *
 *  DRAW FIRST. For every function, draw the nodes as boxes and the next
 *  pointers as arrows. Work out which arrows change, and in what order,
 *  before writing any code.
 * ============================================================================
 */

#include <stdlib.h>   /* malloc, free, NULL */
#include "c_list.h"


/* ============================================================================
 *  HELPER FUNCTIONS (optional, write your own here)
 * ============================================================================
 *
 *  Mark every helper `static`: it isn't declared in c_list.h, so it is
 *  private to this file. Write each helper ABOVE the functions that use it.
 *  An unused static helper is a warning, and a warning fails the build, so
 *  only keep the helpers you actually call.
 *
 *  Ideas that several functions below can share:
 *
 *   - static Node *new_node(int value, Node *next)
 *       malloc a Node, fill in value and next, return it (or NULL if malloc
 *       fails). Useful in: push_front, push_back, insert_at.
 *       Careful: sizeof(Node), not sizeof(Node *). What's the difference?
 *
 *   - static Node *node_at(const List *list, size_t index)
 *       Walk `index` steps from the head and return that node. Only call it
 *       with an index you've already checked is valid.
 *       Useful in: push_back, get, insert_at, remove_at.
 *       insert_at and remove_at need the node BEFORE the position: which
 *       index is that?
 */




/* ============================================================================
 *  LEVEL 1: creating, adding, destroying
 * ============================================================================ */

/*
 *  list_create: allocate a new, empty List (head NULL, size 0) and return it.
 *  Return NULL if malloc fails.
 *
 *  HINTS
 *   - Why must the List come from malloc? What happens to a local variable
 *     when the function returns?
 *   - Set BOTH fields. Memory from malloc is not zeroed for you.
 *
 *  YOUR NOTES
 *   Approach:
 *
 *   Edge cases I handled:
 *
 *   How I tested it:
 *
 */
List *list_create(void)
{
    /* TODO */
    return NULL;
}


/*
 *  list_destroy: free every node, then the List itself.
 *
 *  HINTS
 *   - Walk the list freeing each node. But once a node is freed, you may
 *     not read its next pointer. What do you need to save first?
 *   - Free the List last. Why last, and not first?
 *   - Does your loop work for an empty list without a special case?
 *
 *  YOUR NOTES
 *   Approach:
 *
 *   Edge cases I handled:
 *
 *   How I tested it:
 *
 */
void list_destroy(List *list)
{
    (void)list;   /* remove these (void) lines once you use the parameters */

    /* TODO */
}


/*
 *  list_push_front: add a new node holding `value` at the FRONT.
 *  Return 0, or -1 if malloc fails.
 *
 *  HINTS
 *   - Two arrows change: the new node's next, and list->head. Which one
 *     must you set first so the rest of the list isn't lost?
 *   - Does the empty list need special handling here? Draw it and see.
 *   - Don't forget list->size.
 *
 *  YOUR NOTES
 *   Approach:
 *
 *   Edge cases I handled:
 *
 *   How I tested it:
 *
 */
int list_push_front(List *list, int value)
{
    (void)list;
    (void)value;

    /* TODO */
    return -1;
}


/*
 *  list_push_back: add a new node holding `value` at the END.
 *  Return 0, or -1 if malloc fails.
 *
 *  HINTS
 *   - You need the LAST node: the one whose next is NULL. Write a loop that
 *     stops ON the last node, not after walking off the end.
 *   - The empty list has no last node. What should happen instead?
 *   - The new node's next must be NULL. Who sets that?
 *
 *  YOUR NOTES
 *   Approach:
 *
 *   Edge cases I handled:
 *
 *   How I tested it:
 *
 */
int list_push_back(List *list, int value)
{
    (void)list;
    (void)value;

    /* TODO */
    return -1;
}


/* ============================================================================
 *  LEVEL 2: reading and removing the front
 * ============================================================================ */

/*
 *  list_get: store the value at position `index` in *out and return 0.
 *  Return -1 if index >= size (list and *out unchanged).
 *
 *  HINTS
 *   - index is a size_t, which can't be negative. One comparison against
 *     list->size rejects every invalid index. Do it before walking.
 *   - Once the index is known to be valid, how many steps from the head do
 *     you take?
 *   - The list is const here: you read it, you never change it.
 *
 *  YOUR NOTES
 *   Approach:
 *
 *   Edge cases I handled:
 *
 *   How I tested it:
 *
 */
int list_get(const List *list, size_t index, int *out)
{
    (void)list;
    (void)index;
    (void)out;

    /* TODO */
    return -1;
}


/*
 *  list_pop_front: remove the FIRST node, store its value in *out, free it,
 *  and return 0. Return -1 if the list is empty (*out unchanged).
 *
 *  HINTS
 *   - You need three things from the old head: its value, its next pointer,
 *     and the node itself (to free it). Write down the order you'll use them,
 *     and make sure free comes AFTER every read.
 *   - What is list->head after popping the only node? Does your code produce
 *     that without a special case?
 *
 *  YOUR NOTES
 *   Approach:
 *
 *   Edge cases I handled:
 *
 *   How I tested it:
 *
 */
int list_pop_front(List *list, int *out)
{
    (void)list;
    (void)out;

    /* TODO */
    return -1;
}


/* ============================================================================
 *  LEVEL 3: relinking anywhere in the list
 * ============================================================================ */

/*
 *  list_insert_at: insert a new node holding `value` at position `index`.
 *  Valid indexes are 0 .. size (index == size adds at the end).
 *  Return 0, or -1 if index > size or malloc fails (list unchanged).
 *
 *  HINTS
 *   - Check the index BEFORE calling malloc. Otherwise, what happens to the
 *     new node when the index turns out to be invalid?
 *   - For index 0 the new node becomes the head: you already wrote a function
 *     that does exactly that.
 *   - Otherwise, find the node BEFORE the position (index - 1). Then set the
 *     new node's next FIRST, and the previous node's next SECOND. Try the
 *     other order on paper: what gets lost?
 *
 *  YOUR NOTES
 *   Approach:
 *
 *   Edge cases I handled:
 *
 *   How I tested it:
 *
 */
int list_insert_at(List *list, size_t index, int value)
{
    (void)list;
    (void)index;
    (void)value;

    /* TODO */
    return -1;
}


/*
 *  list_remove_at: remove the node at position `index`, store its value in
 *  *out, free it, and return 0. Valid indexes are 0 .. size - 1.
 *  Return -1 if index >= size (list and *out unchanged).
 *
 *  HINTS
 *   - Removing the head is different: there's no previous node, so
 *     list->head changes instead. You already wrote a function for that.
 *   - Otherwise, find the node BEFORE the one to remove, make it skip over
 *     the target, and only then free the target.
 *   - Removing the LAST node is a good case to trace by hand: what is the
 *     previous node's next afterwards?
 *
 *  YOUR NOTES
 *   Approach:
 *
 *   Edge cases I handled:
 *
 *   How I tested it:
 *
 */
int list_remove_at(List *list, size_t index, int *out)
{
    (void)list;
    (void)index;
    (void)out;

    /* TODO */
    return -1;
}


/*
 *  list_reverse: reverse the list IN PLACE by changing next pointers.
 *  Don't create or free nodes, and don't change any value: the same nodes
 *  must end up in the opposite order. (Swapping values fails the tests.)
 *
 *  HINTS
 *   - Walk the list once, turning each arrow around as you go.
 *   - You need three pointers: the node BEFORE the current one, the CURRENT
 *     node, and the node AFTER it. Why must you save "after" before turning
 *     the current node's arrow around?
 *   - What should the first node's next become? And what is the new head
 *     when the loop ends?
 *   - Trace your loop on paper for [], [4], and [3 7 5] before running it.
 *
 *  YOUR NOTES
 *   Approach:
 *
 *   Edge cases I handled:
 *
 *   How I tested it:
 *
 */
void list_reverse(List *list)
{
    (void)list;

    /* TODO */
}

/*
 * main.c: YOUR test program. Not submitted and not graded.
 *
 * It includes the same header as c_list.c, so it can call your functions.
 * `make` compiles both files and links them into one program, ./c_list.
 *
 * Add your own tests here, especially for the edge cases in the README:
 * the empty list, a one-node list, the first and last positions, and
 * invalid indexes. Print the list after every step and compare it with
 * what you drew on paper.
 */

#include <stdio.h>
#include "c_list.h"

/* Prints a list like the grader does: [3 7 5] (size 3) */
static void print_list(const char *label, const List *list)
{
    printf("%s: [", label);
    for (const Node *node = list->head; node != NULL; node = node->next) {
        printf("%d", node->value);
        if (node->next != NULL)
            printf(" ");
    }
    printf("] (size %zu)\n", list->size);
}

int main(void)
{
    List *list = list_create();
    if (list == NULL) {
        printf("list_create returned NULL: write list_create first!\n");
        return 1;
    }
    print_list("after create", list);                 /* expected: [] (size 0) */

    list_push_back(list, 3);
    list_push_back(list, 7);
    list_push_back(list, 5);
    print_list("after push_back 3, 7, 5", list);      /* expected: [3 7 5] (size 3) */

    list_push_front(list, 1);
    print_list("after push_front 1", list);           /* expected: [1 3 7 5] (size 4) */

    int value = 0;
    int result = list_get(list, 2, &value);
    printf("get 2: returns %d, value %d\n", result, value);   /* expected: 0, 7 */

    result = list_get(list, 4, &value);
    printf("get 4: returns %d\n", result);                    /* expected: -1 */

    list_insert_at(list, 2, 9);
    print_list("after insert_at 2, 9", list);         /* expected: [1 3 9 7 5] (size 5) */

    list_remove_at(list, 0, &value);
    printf("remove_at 0 removed %d\n", value);                /* expected: 1 */
    print_list("after remove_at 0", list);            /* expected: [3 9 7 5] (size 4) */

    list_reverse(list);
    print_list("after reverse", list);                /* expected: [5 7 9 3] (size 4) */

    list_pop_front(list, &value);
    printf("pop_front removed %d\n", value);                  /* expected: 5 */
    print_list("after pop_front", list);              /* expected: [7 9 3] (size 3) */

    list_destroy(list);   /* always finish by destroying every list you create */
    return 0;
}

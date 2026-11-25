/*
 * driver.c - autograder driver for the Linked List assignment.
 *
 * PROVIDED at grading time together with c_list.h (files.provided). It
 * #includes the student's c_list.c, so it is the only file passed to gcc:
 *
 *     gcc -std=c11 -Wall -Wextra -Werror -g driver.c -o c_list
 *
 * ---------------------------------------------------------------------------
 * INPUT FORMAT (stdin): one command per line, words separated by spaces.
 * ---------------------------------------------------------------------------
 *   build 3 7 5        driver builds the list [3 7 5] ITSELF (no student code)
 *   build              driver builds an empty list
 *   create             calls list_create()
 *   destroy            calls list_destroy()
 *   push_front 4       push_back 4
 *   get 1              pop_front              remove_at 1
 *   insert_at 1 9      (index, then value)
 *   reverse
 *
 * Blank lines and lines starting with # are ignored. Each test starts with
 * build or create. Example (tests.yaml):  stdin: "build 3 7 5\nremove_at 1\n"
 *
 * ---------------------------------------------------------------------------
 * WHAT THE DRIVER CHECKS
 * ---------------------------------------------------------------------------
 * - malloc/free inside c_list.c are replaced with versions that record every
 *   allocation. free() never really frees: it fills the block with 0xDB
 *   bytes and keeps it, so reading a freed node gives garbage values or a
 *   crash on every run, never a lucky correct answer.
 * - After every step, the driver walks the list itself and prints it. Before
 *   following any pointer it checks that the pointer is a live allocation
 *   made by malloc, big enough for a Node, so a broken list is reported
 *   instead of crashing the driver. It also reports cycles and a wrong size.
 * - reverse: the same nodes must come back in the opposite order, and no
 *   memory may be allocated or freed.
 * - At the end, the driver frees whatever list is left and reports leaks.
 *   Double frees and frees of non-malloc pointers are reported immediately.
 * - Any other library call inside c_list.c prints FORBIDDEN and exits (3).
 *
 * Malformed TEST input (an instructor mistake) prints "DRIVER ERROR" to
 * stderr and exits with status 2.
 */

/* ===== 1. Headers a student might include, BEFORE the bans. ============== */
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "c_list.h"

/* ===== 2. The allocation tracker (malloc/free stand-ins). ================= */

typedef struct {
    void  *ptr;
    size_t size;
    bool   freed;
} AgBlock;

static AgBlock *ag_blocks = NULL;
static size_t   ag_nblocks = 0, ag_capblocks = 0;
static long     ag_alloc_events = 0, ag_free_events = 0;

static AgBlock *ag_find(const void *p)
{
    for (size_t i = 0; i < ag_nblocks; i++)
        if (ag_blocks[i].ptr == p)
            return &ag_blocks[i];
    return NULL;
}

static void *ag_track_malloc(size_t size)
{
    if (ag_nblocks == ag_capblocks) {
        ag_capblocks = ag_capblocks ? ag_capblocks * 2 : 64;
        ag_blocks = realloc(ag_blocks, ag_capblocks * sizeof *ag_blocks);
        if (ag_blocks == NULL) {
            fprintf(stderr, "DRIVER ERROR: out of memory\n");
            exit(2);
        }
    }
    void *p = malloc(size ? size : 1);
    if (p == NULL) {
        fprintf(stderr, "DRIVER ERROR: out of memory\n");
        exit(2);
    }
    ag_blocks[ag_nblocks++] = (AgBlock){p, size, false};
    ag_alloc_events++;
    return p;
}

/* Was this pointer value read out of freed (0xDB-filled) memory? */
static bool ag_is_poison(const void *p)
{
    unsigned char bytes[sizeof p];
    memcpy(bytes, &p, sizeof p);
    for (size_t i = 0; i < sizeof p; i++)
        if (bytes[i] != 0xDB)
            return false;
    return true;
}

static void ag_track_free(void *p)
{
    if (p == NULL)
        return;                         /* free(NULL) is allowed */
    AgBlock *b = ag_find(p);
    if (b == NULL) {
        if (ag_is_poison(p))
            printf("memory error: free() was called on a garbage pointer "
                   "read from freed memory (did you use a node after "
                   "freeing it?)\n");
        else
            printf("memory error: free() was called on a pointer that did "
                   "not come from malloc\n");
        fflush(stdout);
        exit(4);
    }
    if (b->freed) {
        printf("memory error: the same memory was freed twice "
               "(double free)\n");
        fflush(stdout);
        exit(4);
    }
    memset(b->ptr, 0xDB, b->size);      /* poison: never really freed */
    b->freed = true;
    ag_free_events++;
}

static size_t ag_live_count(void)
{
    size_t n = 0;
    for (size_t i = 0; i < ag_nblocks; i++)
        if (!ag_blocks[i].freed)
            n++;
    return n;
}

/* Is p the start of a live allocation of at least `need` bytes? */
static bool ag_is_live(const void *p, size_t need, bool *too_small)
{
    AgBlock *b = ag_find(p);
    if (too_small)
        *too_small = false;
    if (b == NULL || b->freed)
        return false;
    if (b->size < need) {
        if (too_small)
            *too_small = true;
        return false;
    }
    return true;
}

/* ===== 3. Stand-ins for banned functions. ================================= */

static _Noreturn void ag_forbidden(const char *name)
{
    printf("FORBIDDEN: your code called %s(), which is not allowed in this "
           "assignment\n", name);
    fflush(stdout);
    exit(3);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-function"

#define AG_BAN(ret, name, params) \
    static ret ag_banned_##name params { ag_forbidden(#name); }

/* <string.h> */
AG_BAN(size_t, strlen,  (const char *s))
AG_BAN(char *, strcpy,  (char *d, const char *s))
AG_BAN(char *, strncpy, (char *d, const char *s, size_t n))
AG_BAN(int,    strcmp,  (const char *a, const char *b))
AG_BAN(void *, memcpy,  (void *d, const void *s, size_t n))
AG_BAN(void *, memmove, (void *d, const void *s, size_t n))
AG_BAN(void *, memset,  (void *d, int c, size_t n))
AG_BAN(int,    memcmp,  (const void *a, const void *b, size_t n))
/* <stdio.h> */
AG_BAN(int, printf,   (const char *f, ...))
AG_BAN(int, fprintf,  (FILE *fp, const char *f, ...))
AG_BAN(int, sprintf,  (char *d, const char *f, ...))
AG_BAN(int, snprintf, (char *d, size_t n, const char *f, ...))
AG_BAN(int, puts,     (const char *s))
AG_BAN(int, fputs,    (const char *s, FILE *fp))
AG_BAN(int, putchar,  (int c))
AG_BAN(int, scanf,    (const char *f, ...))
AG_BAN(int, getchar,  (void))
/* <stdlib.h>: malloc and free are allowed (tracked), these are not */
AG_BAN(void *, calloc,  (size_t n, size_t sz))
AG_BAN(void *, realloc, (void *p, size_t n))
AG_BAN(void,   exit,    (int status))
AG_BAN(void,   abort,   (void))

#pragma GCC diagnostic pop

/* ===== 4. Rename calls, compile the student's file, then restore. ========= */
#undef strlen
#undef strcpy
#undef strncpy
#undef strcmp
#undef memcpy
#undef memmove
#undef memset
#undef memcmp
#undef printf
#undef fprintf
#undef sprintf
#undef snprintf
#undef puts
#undef fputs
#undef putchar
#undef scanf
#undef getchar
#undef calloc
#undef realloc
#undef exit
#undef abort
#undef malloc
#undef free

#define strlen   ag_banned_strlen
#define strcpy   ag_banned_strcpy
#define strncpy  ag_banned_strncpy
#define strcmp   ag_banned_strcmp
#define memcpy   ag_banned_memcpy
#define memmove  ag_banned_memmove
#define memset   ag_banned_memset
#define memcmp   ag_banned_memcmp
#define printf   ag_banned_printf
#define fprintf  ag_banned_fprintf
#define sprintf  ag_banned_sprintf
#define snprintf ag_banned_snprintf
#define puts     ag_banned_puts
#define fputs    ag_banned_fputs
#define putchar  ag_banned_putchar
#define scanf    ag_banned_scanf
#define getchar  ag_banned_getchar
#define calloc   ag_banned_calloc
#define realloc  ag_banned_realloc
#define exit     ag_banned_exit
#define abort    ag_banned_abort
#define malloc   ag_track_malloc
#define free     ag_track_free
#define main     ag_student_main     /* a leftover main() in c_list.c is harmless */

#include "c_list.c"

#undef strlen
#undef strcpy
#undef strncpy
#undef strcmp
#undef memcpy
#undef memmove
#undef memset
#undef memcmp
#undef printf
#undef fprintf
#undef sprintf
#undef snprintf
#undef puts
#undef fputs
#undef putchar
#undef scanf
#undef getchar
#undef calloc
#undef realloc
#undef exit
#undef abort
#undef malloc
#undef free
#undef main

/* ===== 5. The required signatures (a changed one fails to compile). ====== */
List *list_create(void);
void  list_destroy(List *list);
int   list_push_front(List *list, int value);
int   list_push_back(List *list, int value);
int   list_get(const List *list, size_t index, int *out);
int   list_pop_front(List *list, int *out);
int   list_insert_at(List *list, size_t index, int value);
int   list_remove_at(List *list, size_t index, int *out);
void  list_reverse(List *list);

/* ===== 6. Driver internals (all prefixed ag_). ============================ */

#define AG_MAX_LINE   4096
#define AG_MAX_NODES  10000
#define AG_SENTINEL   424242       /* *out starts as this; tests never use it */

static List *ag_list = NULL;       /* the list the test works on */
static bool  ag_have_list = false;

static _Noreturn void ag_die(const char *msg)
{
    fprintf(stderr, "DRIVER ERROR: %s\n", msg);
    exit(2);
}

/* Walks the list without trusting it. Fills nodes[] (up to cap) and returns
 * the node count, or -1 if the list is broken (a message was printed). */
static long ag_walk(Node **nodes, size_t cap, bool quiet)
{
    long count = 0;
    Node *n = ag_list->head;
    while (n != NULL) {
        bool small;
        if (!ag_is_live(n, sizeof(Node), &small)) {
            if (!quiet) {
                if (small)
                    puts("list contains a node allocated with too few bytes "
                         "(sizeof(Node *) instead of sizeof(Node)?)");
                else if (ag_is_poison(n))
                    puts("list contains a garbage pointer read from freed "
                         "memory (was a node used after it was freed?)");
                else
                    puts("list contains a pointer to freed or invalid memory");
            }
            return -1;
        }
        for (long i = 0; i < count && (size_t)i < cap; i++) {
            if (nodes[i] == n) {
                if (!quiet)
                    puts("list loops back on itself");
                return -1;
            }
        }
        if ((size_t)count < cap)
            nodes[count] = n;
        count++;
        if (count > AG_MAX_NODES) {
            if (!quiet)
                puts("list loops back on itself");
            return -1;
        }
        n = n->next;
    }
    return count;
}

/* Prints "list: [a b c] (size N)" plus any size problem. */
static void ag_print_list(const char *label)
{
    static Node *nodes[AG_MAX_NODES + 1];
    printf("%s: ", label);
    long count = ag_walk(nodes, AG_MAX_NODES + 1, true);
    if (count < 0) {
        ag_walk(nodes, AG_MAX_NODES + 1, false);   /* print the reason */
        return;
    }
    putchar('[');
    for (long i = 0; i < count; i++) {
        if (i > 0)
            putchar(' ');
        printf("%d", nodes[i]->value);
    }
    printf("] (size %zu)\n", ag_list->size);
    if (ag_list->size != (size_t)count)
        printf("size is %zu but the list has %ld nodes\n",
               ag_list->size, count);
}

static void ag_need_list(void)
{
    if (!ag_have_list)
        ag_die("command needs a list: start the test with build or create");
}

/* Parses argv[i] as an int (dies on bad test input). */
static long ag_arg(int argc, char **argv, int i)
{
    if (i >= argc)
        ag_die("missing number");
    char *end;
    long v = strtol(argv[i], &end, 10);
    if (*end != '\0' || v < INT_MIN || v > INT_MAX)
        ag_die("arguments must be ints");
    return v;
}

static void ag_no_more(int argc, int used)
{
    if (argc > used)
        ag_die("too many arguments");
}

static void ag_print_out(int ret, int out)
{
    if (out == AG_SENTINEL)
        printf("returns %d, out unchanged\n", ret);
    else
        printf("returns %d, out = %d\n", ret, out);
}

/* ===== 7. Commands. ======================================================= */

static void ag_build(int argc, char **argv)
{
    if (ag_have_list)
        ag_die("build must come first");
    ag_list = ag_track_malloc(sizeof(List));
    ag_list->head = NULL;
    ag_list->size = 0;
    Node *tail = NULL;
    for (int i = 1; i < argc; i++) {
        long v = ag_arg(argc, argv, i);
        Node *n = ag_track_malloc(sizeof(Node));
        n->value = (int)v;
        n->next = NULL;
        if (tail)
            tail->next = n;
        else
            ag_list->head = n;
        tail = n;
        ag_list->size++;
    }
    ag_have_list = true;
    ag_print_list("start");
}

static void ag_create(void)
{
    if (ag_have_list)
        ag_die("create must come first");
    printf("list_create -> ");
    fflush(stdout);
    List *l = list_create();
    if (l == NULL) {
        puts("returned NULL");
        return;
    }
    bool small;
    if (!ag_is_live(l, sizeof(List), &small)) {
        puts(small ? "returned memory too small for a List "
                     "(sizeof(List *) instead of sizeof(List)?)"
                   : "returned a pointer that did not come from malloc");
        return;
    }
    puts("returned a new list");
    ag_list = l;
    ag_have_list = true;
    ag_print_list("list");
}

static void ag_destroy(void)
{
    ag_need_list();
    printf("list_destroy -> ");
    fflush(stdout);
    list_destroy(ag_list);
    puts("done");
    ag_list = NULL;
    ag_have_list = false;
}

static void ag_push(int argc, char **argv, bool front)
{
    ag_need_list();
    long v = ag_arg(argc, argv, 1);
    ag_no_more(argc, 2);
    printf("%s %ld -> ", front ? "push_front" : "push_back", v);
    fflush(stdout);
    int r = front ? list_push_front(ag_list, (int)v)
                  : list_push_back(ag_list, (int)v);
    printf("returns %d\n", r);
    ag_print_list("list");
}

static void ag_get(int argc, char **argv)
{
    ag_need_list();
    long i = ag_arg(argc, argv, 1);
    ag_no_more(argc, 2);
    if (i < 0)
        ag_die("index must not be negative");
    int out = AG_SENTINEL;
    printf("get %ld -> ", i);
    fflush(stdout);
    int r = list_get(ag_list, (size_t)i, &out);
    ag_print_out(r, out);
    ag_print_list("list");
}

static void ag_pop_front(void)
{
    ag_need_list();
    int out = AG_SENTINEL;
    printf("pop_front -> ");
    fflush(stdout);
    int r = list_pop_front(ag_list, &out);
    ag_print_out(r, out);
    ag_print_list("list");
}

static void ag_insert_at(int argc, char **argv)
{
    ag_need_list();
    long i = ag_arg(argc, argv, 1);
    long v = ag_arg(argc, argv, 2);
    ag_no_more(argc, 3);
    if (i < 0)
        ag_die("index must not be negative");
    printf("insert_at %ld %ld -> ", i, v);
    fflush(stdout);
    int r = list_insert_at(ag_list, (size_t)i, (int)v);
    printf("returns %d\n", r);
    ag_print_list("list");
}

static void ag_remove_at(int argc, char **argv)
{
    ag_need_list();
    long i = ag_arg(argc, argv, 1);
    ag_no_more(argc, 2);
    if (i < 0)
        ag_die("index must not be negative");
    int out = AG_SENTINEL;
    printf("remove_at %ld -> ", i);
    fflush(stdout);
    int r = list_remove_at(ag_list, (size_t)i, &out);
    ag_print_out(r, out);
    ag_print_list("list");
}

static void ag_reverse(void)
{
    static Node *before[AG_MAX_NODES + 1];
    static int   values[AG_MAX_NODES + 1];
    static Node *after[AG_MAX_NODES + 1];
    ag_need_list();

    long n_before = ag_walk(before, AG_MAX_NODES + 1, true);
    for (long i = 0; i < n_before; i++)
        values[i] = before[i]->value;
    long allocs = ag_alloc_events, frees = ag_free_events;

    printf("reverse -> ");
    fflush(stdout);
    list_reverse(ag_list);
    puts("done");
    ag_print_list("list");

    if (ag_alloc_events != allocs || ag_free_events != frees) {
        puts("list_reverse allocated or freed memory");
        return;
    }
    long n_after = ag_walk(after, AG_MAX_NODES + 1, true);
    if (n_before < 0 || n_after != n_before)
        return;                     /* already visible in the printed list */
    bool same_nodes = true;
    for (long i = 0; i < n_after; i++)
        if (after[i] != before[n_after - 1 - i])
            same_nodes = false;
    bool values_kept = true;
    for (long i = 0; i < n_after; i++)
        if (before[i]->value != values[i])
            values_kept = false;
    if (!same_nodes || !values_kept)
        puts("values were changed instead of relinking nodes");
}

/* Frees what is left of the list and reports leaks. */
static void ag_finish(void)
{
    if (ag_have_list) {
        static Node *nodes[AG_MAX_NODES + 1];
        long count = ag_walk(nodes, AG_MAX_NODES + 1, true);
        if (count < 0) {
            puts("memory: not checked (the list is broken)");
            return;
        }
        for (long i = 0; i < count; i++)
            ag_track_free(nodes[i]);
        ag_track_free(ag_list);
    }
    size_t leaked = ag_live_count();
    if (leaked == 0)
        puts("memory: OK (every allocation freed exactly once)");
    else
        printf("memory: %zu allocation(s) never freed (leak)\n", leaked);

    /* Really release everything, so tools like valgrind see a clean exit. */
    for (size_t i = 0; i < ag_nblocks; i++)
        free(ag_blocks[i].ptr);
    free(ag_blocks);
}

/* ===== 8. Command loop. =================================================== */

int main(void)
{
    char line[AG_MAX_LINE];
    char *argv[AG_MAX_LINE / 2 + 1];
    while (fgets(line, sizeof line, stdin) != NULL) {
        line[strcspn(line, "\r\n")] = '\0';
        int argc = 0;
        for (char *tok = strtok(line, " \t"); tok; tok = strtok(NULL, " \t"))
            argv[argc++] = tok;
        if (argc == 0 || argv[0][0] == '#')
            continue;
        const char *cmd = argv[0];
        bool no_args = true;

        if      (strcmp(cmd, "build") == 0)      { ag_build(argc, argv); no_args = false; }
        else if (strcmp(cmd, "create") == 0)     ag_create();
        else if (strcmp(cmd, "destroy") == 0)    ag_destroy();
        else if (strcmp(cmd, "push_front") == 0) { ag_push(argc, argv, true); no_args = false; }
        else if (strcmp(cmd, "push_back") == 0)  { ag_push(argc, argv, false); no_args = false; }
        else if (strcmp(cmd, "get") == 0)        { ag_get(argc, argv); no_args = false; }
        else if (strcmp(cmd, "pop_front") == 0)  ag_pop_front();
        else if (strcmp(cmd, "insert_at") == 0)  { ag_insert_at(argc, argv); no_args = false; }
        else if (strcmp(cmd, "remove_at") == 0)  { ag_remove_at(argc, argv); no_args = false; }
        else if (strcmp(cmd, "reverse") == 0)    ag_reverse();
        else
            ag_die("unknown command");
        if (no_args)
            ag_no_more(argc, 1);
        fflush(stdout);
    }
    ag_finish();
    return 0;
}

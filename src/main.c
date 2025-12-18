#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "b_tree.h"
#include "citer.h"
#include "my_string.h"
#include "posints.h"

#define REPO_URL "https://github.com/nasccped/citer-example"
#define HELP_FLAG "--help"
#define RESET "\x1b[0m"
#define RED "\x1b[91m"
#define GREEN "\x1b[92m"
#define YELLOW "\x1b[93m"
#define MAGENTA "\x1b[95m"
#define CYAN "\x1b[96m"
#define WHITE "\x1b[97m"

typedef enum { NOTE, ERROR, HELP } Tag;
typedef void (*PartFunction)(void);

void usage_tip(void);
void print_help(void);
void print_title(int);
void print_tag(FILE *, Tag, const char *, ...);
void part1(void);
void part2(void);
void part3(void);
void part4(void);

int float_comp(float *, float *);
void ghost_free(void);

static PartFunction parts[] = { part1, part2, part3, part4 };
static char *descriptions[] = {
    "CIterator constructor and destructor",
    "CIterator set data and create from",
    "CIterator cursor move",
    "CIterator getters",
};

int main(int argc, char *argv[]) {
    // when passing unexpected amount of args
    if (argc != 2) {
        print_tag(stderr, ERROR, "couldn't run with the provided args: [");
        if (argc == 1)
            fprintf(stderr, "%s<none>%s", RED, RESET);
        else {
            for (int i = 1; i < argc; i++)
                fprintf(stderr, "%s%s%s%s", YELLOW, argv[i], RESET, i == argc - 1 ? "" : ", ");
        }
        fprintf(stderr, "]\n\n");
        usage_tip();
        return 1;
    }
    char *arg = argv[1];
    int target = 0;
    if (strcmp(arg, HELP_FLAG) == 0) {
        print_help();
        return 0;
    } else {
        for (size_t i = 0; i < strlen(arg); i++) {
            if (arg[i] < '0' || arg[i] > '9') {
                print_tag(stderr,
                          ERROR,
                          "couldn't run with the provided arg (%s%s%s)\n\n",
                          RED,
                          arg,
                          RESET);
                usage_tip();
                return 1;
            }
            target *= 10;
            target += arg[i] - '0';
        }
    }
    if ((size_t)target > sizeof(parts) / sizeof(parts[0])) {
        print_tag(stderr,
                  ERROR,
                  "the target part (%s%d%s) isn't available. Consider running the\n"
                  "program with the `%s%s%s` flag.\n",
                  GREEN,
                  target,
                  RESET,
                  CYAN,
                  HELP_FLAG,
                  RESET);
        return 1;
    }
    PartFunction func = parts[target - 1];
    print_title(target);
    func();
    return 0;
}

void usage_tip(void) {
    fprintf(stderr, "Usage tip: %s<BINARY_PATH>%s <ARG>%s\n\n", GREEN, CYAN, RESET);
    fprintf(stderr, "%s<ARG>%s can be:\n", CYAN, RESET);
    fprintf(
        stderr, "  %s%-10s%s prints parts index and their descriptions.\n", CYAN, HELP_FLAG, RESET);
    fprintf(stderr, "  %s%-10s%s calls the part function.\n", CYAN, "<NUMERIC>", RESET);
}

void print_help(void) {
    print_tag(stdout,
              HELP,
              "CIterator project implements the iterator abstraction in\n"
              "a generic way (pure C).\n\n");
    printf("It also contains separated parts which you can access\n");
    printf("by running %s<BINARY_PATH>%s <PART_INDEX>%s:\n\n", GREEN, CYAN, RESET);
    for (size_t i = 0; i < sizeof(parts) / sizeof(parts[0]); i++) {
        printf("  %s%02ld.%s %s\n", CYAN, i + 1, RESET, descriptions[i]);
    }
    printf("\n");
    print_tag(stdout,
              NOTE,
              "did you find any bug? Consider oppening an %sissue%s\n"
              "at %s%s%s!\n",
              GREEN,
              RESET,
              CYAN,
              REPO_URL,
              RESET);
}

void print_title(int index) {
    char *title = descriptions[index - 1];
    printf("%s%02d%s. %s\n", CYAN, index, WHITE, title);
    for (size_t i = 0; i < strlen(title) + 4; i++)
        printf("=");
    printf("%s\n\n", RESET);
}

void print_tag(FILE *f, Tag t, const char *format, ...) {
    va_list args;
    va_start(args, format);
    const char *color, *tag_str;
    switch (t) {
    case NOTE:
        color = CYAN;
        tag_str = "note";
        break;
    case ERROR:
        color = RED;
        tag_str = "error";
        break;
    case HELP:
        color = MAGENTA;
        tag_str = "help";
        break;
    default:
        color = RESET;
    }
    fprintf(f, "%s%s%s: ", color, tag_str, RESET);
    vfprintf(f, format, args);
    va_end(args);
}

void part1() {
    char *is_null = "is null", *is_not_null = "is not null";
    CIterator *citer = NULL;
    printf("We can allocate the CIterator pointer using a\n");
    printf("constructor like function:\n\n");
    printf("  CIterator pointer before `%sciterator_new%s` function: %s%s%s\n",
           CYAN,
           RESET,
           citer ? GREEN : RED,
           citer ? is_not_null : is_null,
           RESET);
    citer = citerator_new();
    printf("  CIterator pointer after `%sciterator_new%s` function: %s%s%s\n",
           CYAN,
           RESET,
           citer ? GREEN : RED,
           citer ? is_not_null : is_null,
           RESET);
    printf("\nSince CIterator holds a %svoid **%s data, we can push new\n", CYAN, RESET);
    printf("values without allocating new memory for iterator itself.\n\n");
    printf("When the CIterator use is done, we must free the memory by\n");
    citerator_destroy(citer);
    printf("using the `%sciterator_destroy%s` function, and now, the CIterator\n", CYAN, RESET);
    printf("pointer %s%s%s.\n\n", citer ? GREEN : RED, citer ? is_null : is_not_null, RESET);
    print_tag(stdout,
              NOTE,
              "we can also use the `%sciterator_go_next_or_free%s` function\n"
              "to auto free the CIterator when the iteration is done, but we'll\n"
              "talk about it later.\n",
              CYAN,
              RESET);
}

void part2() {
    void (*func_alias1)(CIterator *, void *) =
        (void (*)(CIterator *, void *))push_string_to_citerator;
    CIterator *(*func_alias2)(void *) = (CIterator * (*)(void *)) new_citerator_from_string;
    printf("There's two ways to set data into a CIterator struct!\n\n");
    printf("%s1%s. We can use `%sciterator_set%s` function to set data into\n",
           CYAN,
           RESET,
           CYAN,
           RESET);
    printf("   a %snon NULL%s CIterator pointer:\n\n   > ", GREEN, RESET);
    CIterator *citer = citerator_new();
    for (citerator_set(citer, "data", func_alias1); !citerator_is_done(citer);
         citerator_go_next(citer))
        printf("[ %s%c%s ]", GREEN, *(char *)citer->current, RESET);
    citerator_destroy(citer);
    printf("\n\n");
    printf("%s2%s.We can use the `%sciterator_new_from%s` to create a new CIterator\n",
           CYAN,
           RESET,
           CYAN,
           RESET);
    printf("   pointer based on the given type:\n\n   > ");
    for (citer = citerator_new_from("other data", func_alias2); !citerator_is_done(citer);
         citerator_go_next(citer))
        printf("[ %s%c%s ]", GREEN, *(char *)citer->current, RESET);
    citerator_destroy(citer);
    printf("\n\n");
    printf("The key difference is that mem realloc can be %sexpensive%s when\n", RED, RESET);
    printf("handling big amount of data.\n\n");
    printf("Instead of %srealloc%s the CIterator mem, we can just %sreuse%s\n",
           YELLOW,
           RESET,
           GREEN,
           RESET);
    printf("an already allocated mem (with `%sciterator_set%s`) and\n", CYAN, RESET);
    printf("avoid unnecessary alloc.\n");
}

void part3() {
    CIterator *citer = citerator_new();
    void (*func_alias1)(CIterator *, void *) =
        (void (*)(CIterator *, void *))push_posints_into_citerator;
    printf("There's three different ways of CIterator cursor moving:\n\n");
    printf("%s> citerator_go_next%s: move the `current` pointer until the\n", GREEN, RESET);
    printf("  end of the iterator queue (don't free the address holder):\n");
    printf("    %sints%s -> ", YELLOW, RESET);
    for (citerator_set(citer, (int[]){ 2, 3, 5, 7, -1 }, func_alias1); !citerator_is_done(citer);
         citerator_go_next(citer))
        printf("%d ", *(int *)citer->current);
    printf("\n\n  Since the addresses were not freed, we can recover the\n");
    printf("  iteration by using the `%sciterator_reset%s` function:\n", CYAN, RESET);
    printf("    %ssame ints%s -> ", YELLOW, RESET);
    for (citerator_reset(citer); !citerator_is_done(citer); citerator_go_next(citer))
        printf("%d ", *(int *)citer->current);
    citerator_clear(citer);
    printf("\n\n");
    printf("%s> citerator_go_next_and_consume%s: move the `current` pointer\n", GREEN, RESET);
    printf("  until the end of the iterator queue. If the end is\n");
    printf("  reached, the address holder %sis freed%s but the CIterator\n", GREEN, RESET);
    printf("  pointer remains accessible, allowing it to receive new\n");
    printf("  itens:\n");
    printf("    %sints%s -> ", YELLOW, RESET);
    for (citerator_set(citer, (int[]){ 11, 13, 17, -1 }, func_alias1); !citerator_is_done(citer);
         citerator_go_next_and_consume(citer))
        printf("%d ", *(int *)citer->current);
    printf("\n\n  Now, the addresses were %s%s%s but the CIterator pointer\n",
           citer->root_pointer ? RED : CYAN,
           citer->root_pointer ? "not free" : "free",
           RESET);
    printf(
        "  remains %s%s%s!\n\n", citer ? GREEN : RED, citer ? "accessible" : "inaccessible", RESET);
    printf("%s> citerator_go_next_or_free%s: destroys the entire CIterator\n", GREEN, RESET);
    printf("  pointer mem when the iteration is done, allowing us to use\n");
    printf("  the CIterator %sself pointer%s as loop condition:\n", GREEN, RESET);
    printf("    %sints%s -> ", YELLOW, RESET);
    for (citerator_set(citer, (int[]){ 19, 23, 29, -1 }, func_alias1); citer;
         citer = citerator_go_next_or_free(citer))
        printf("%d ", *(int *)citer->current);
    printf("\n\n  And now, ");
    if (!citer)
        printf("the CIterator pointer %sis NULL%s!\n", GREEN, RESET);
    else if (!citer->root_pointer)
        printf("the CIterator pointer %sisn't NULL%s? ._.\n", YELLOW, RESET);
    else
        printf("both CIterator and addresses %saren't NULL%s, wth? o_O\n", RED, RESET);
}

static float MY_FLOATS[] = { 2.0f, 1.0f, 0.5f, 3.14159f, 6.2831f, 1.4142f };

void part4(void) {
    int (*comp)(void *, void *) = (int (*)(void *, void *))float_comp;
    void (*free_func)(void *) = (void (*)(void *))ghost_free;
    BTree *tree = b_tree_new(comp, free_func);
    for (size_t i = 0; i < sizeof(MY_FLOATS) / sizeof(MY_FLOATS[0]); i++)
        b_tree_insert(tree, &MY_FLOATS[i]);
    CIterator *(*into_citer)(void *) = (CIterator * (*)(void *)) new_citerator_from_b_tree;
    CIterator *citer = citerator_new_from((void *)tree, into_citer);
    printf("Manually using the CIterator struct fields can seems a bit hard.\n");
    printf("A good approach is using the provided %sCIterator interface%s\n", GREEN, RESET);
    printf("(%sfunctions%s). Then, all pointer handling logic is (almost) safe\n", CYAN, RESET);
    printf("and fast to use:\n\n");
    while (!citerator_is_done(citer)) {
        printf("  [%s%ld%s: %.3f]\n",
               GREEN,
               citerator_get_index(citer),
               RESET,
               *(float *)citerator_peek(citer));
        citerator_go_next_and_consume(citer);
    }
    printf("\nThe `%sciterator_get_index%s` function returns the index of the\n", CYAN, RESET);
    printf("current iterator item while `%sciterator_peek%s` returns a pointer\n", CYAN, RESET);
    printf("to the current item.\n\n");
    print_tag(stdout,
              NOTE,
              "`%sciterator_peek%s` will return a void pointer which can be\n"
              "%snull%s/needs %sdereferencing%s. Handle it carefully!\n",
              CYAN,
              RESET,
              RED,
              RESET,
              YELLOW,
              RESET);
    citerator_destroy(citer);
    b_tree_destroy(tree);
}

int float_comp(float *self, float *other) {
    if (!self || !other)
        return 0;
    float a = *self, b = *other;
    if (a < b)
        return -1;
    else if (a > b)
        return 1;
    else
        return 0;
}

void ghost_free(void) {}

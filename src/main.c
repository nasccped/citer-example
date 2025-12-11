#include <stdarg.h>
#include <stdio.h>
#include <string.h>

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

typedef enum { NOTE, ERROR, HELP } Tag;
typedef void (*PartFunction)(int);

void usage_tip(void);
void print_help(void);
void print_title(int, const char *, ...);
void print_tag(FILE *, Tag, const char *, ...);
void part1(int);
void part2(int);
void part3(int);

static char TITLE_BUFFER[1024];
static PartFunction parts[] = { part1, part2, part3 };
static char *descriptions[] = { "CIterator constructor and destructor",
                                "CIterator set data and create from",
                                "CIterator cursor move" };

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
    parts[target - 1](target);
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

void print_title(int index, const char *format, ...) {
    va_list args;
    va_start(args, format);
    vsprintf(TITLE_BUFFER, format, args);
    printf("\x1b[96m%02d.\x1b[97m %s\n", index, TITLE_BUFFER);
    for (size_t i = 0; i < strlen(TITLE_BUFFER) + 4; i++)
        printf("=");
    printf("\x1b[0m\n\n");
    va_end(args);
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

void part1(int index) {
    char *is_null = "is null", *is_not_null = "is not null";
    print_title(index, descriptions[index - 1]);
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

void part2(int index) {
    void (*func_alias1)(CIterator *, void *) =
        (void (*)(CIterator *, void *))push_string_to_citerator;
    CIterator *(*func_alias2)(void *) = (CIterator * (*)(void *)) new_citerator_from_string;
    print_title(index, descriptions[index - 1]);
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

void part3(int index) {
    CIterator *citer = citerator_new();
    void (*func_alias1)(CIterator *, void *) =
        (void (*)(CIterator *, void *))push_posints_into_citerator;
    print_title(index, descriptions[index - 1]);
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
        printf("the CIterator pointer %sisn't NULL%s?\n ._.", YELLOW, RESET);
    else
        printf("both CIterator and addresses %saren't NULL%s, wth? o_O\n", RED, RESET);
}

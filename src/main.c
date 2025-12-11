#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "citer.h"
#include "my_string.h"

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
void print_tag(Tag, const char *, ...);
void part1(int);

static char TITLE_BUFFER[1024];
static PartFunction parts[] = { part1 };
static char *descriptions[] = { "CIterator constructor and destructor" };

int main(int argc, char *argv[]) {
    // when passing unexpected amount of args
    if (argc != 2) {
        print_tag(ERROR, "couldn't run with the provided args: [");
        if (argc == 1)
            printf("%s<none>%s", RED, RESET);
        else {
            for (int i = 1; i < argc; i++)
                printf("%s%s%s%s", YELLOW, argv[i], RESET, i == argc - 1 ? "" : ", ");
        }
        printf("]\n\n");
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
                print_tag(
                    ERROR, "couldn't run with the provided arg (%s%s%s)\n\n", RED, arg, RESET);
                usage_tip();
                return 1;
            }
            target *= 10;
            target += arg[i] - '0';
        }
    }
    if ((size_t)target > sizeof(parts) / sizeof(parts[0])) {
        print_tag(ERROR,
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
    printf("Usage tip: %s<BINARY_PATH>%s <ARG>%s\n\n", GREEN, CYAN, RESET);
    printf("%s<ARG>%s can be:\n", CYAN, RESET);
    printf("  %s%-10s%s prints parts index and their descriptions.\n", CYAN, HELP_FLAG, RESET);
    printf("  %s%-10s%s calls the part function.\n", CYAN, "<NUMERIC>", RESET);
}

void print_help(void) {
    print_tag(HELP,
              "CIterator project implements the iterator abstraction in\n"
              "a generic way (pure C).\n\n");
    printf("It also contains separated parts which you can access\n");
    printf("by running %s<BINARY_PATH>%s <PART_INDEX>%s:\n\n", GREEN, CYAN, RESET);
    for (size_t i = 0; i < sizeof(parts) / sizeof(parts[0]); i++) {
        printf("  %s%02ld.%s %s\n", CYAN, i + 1, RESET, descriptions[i]);
    }
    printf("\n");
    print_tag(NOTE,
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

void print_tag(Tag t, const char *format, ...) {
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
    printf("%s%s%s: ", color, tag_str, RESET);
    vprintf(format, args);
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
    print_tag(NOTE,
              "we can also use the `%sciterator_go_next_or_free%s` function\n"
              "to auto free the CIterator when the iteration is done, but we'll\n"
              "talk about it later.\n",
              CYAN,
              RESET);
}

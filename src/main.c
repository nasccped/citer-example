#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2)
        printf("Hello \x1b[92mWorld\x1b[0m!\n");
    else {
        printf("Hello,\x1b[92m");
        for (int i = 1; i < argc; i++)
            printf(" %s", argv[i]);
        printf("\x1b[0m!\n");
    }
    printf("Let's iterate over a?\n");
    char *str = "string";
    for (size_t i = 0; i < strlen(str); i++) {
        printf("%c\n", str[i]);
    }
    return 0;
}

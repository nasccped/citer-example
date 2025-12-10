#include <stdio.h>

int main(int argc, char *argv[]) {
	if (argc < 2)
		printf("Hello \x1b[92mWorld\x1b[0m!\n");
	else {
		printf("Hello,\x1b[92m");
		for (int i = 1; i < argc; i++) printf(" %s", argv[i]);
		printf("\x1b[0m!\n");
	}
	return 0;
}

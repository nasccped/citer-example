CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -Werror -Wshadow -Wformat=2 -Wconversion -Wstrict-prototypes -Wmissing-prototypes
SRC=./src
SRC_FILES=$(wildcard $(SRC)/*.c)
OUT=./out
# .exe extension (windows port)
OUT_FILE=$(OUT)/main.exe

all:
	@echo "make commands:";
	@echo "  - build";
	@echo "  - run   (requires build)";
	@echo "  - clean (requires build)";

build: $(SRC_FILES)
	@if [ ! -d $(OUT) ]; then \
		echo "Creating the \`$(OUT)\` directory..."; \
		mkdir $(OUT); \
	fi;
	@echo "Compiling...";
	@$(CC) $(CFLAGS) -o $(OUT_FILE) $^;
	@echo "Done!"

run: $(OUT_FILE)
	@echo "This Makefile recipe doesn't works anymore!";
	@echo -e "Call the program by using \x1b[92m<BINARY_PATH>\x1b[96m <ARGS...>\x1b[0m";

clean: $(OUT)
	@rm -rf $(OUT);
	@echo "\`$(OUT)\` dir removed...";

fmt: $(SRC_FILES)
	clang-format -i $^
	clang-format -i $(wildcard $(SRC)/*.h)

.PHONY: all build run clean fmt

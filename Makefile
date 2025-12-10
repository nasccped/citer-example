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
	@echo "###### running $< ######";
	@$<;

clean: $(OUT)
	@rm -rf $(OUT);
	@echo "\`$(OUT)\` dir removed...";

fmt: $(SRC_FILES)
	clang-format -i $^
	clang-format -i $(wildcard $(SRC)/*.h)

.PHONY: all build run clean fmt

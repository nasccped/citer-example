citer-example
=============

An Iterator implement in C lang.

## Requirements

This project was built on [Windows WSL](https://github.com/microsoft/WSL), so
you'll need:

- a C compiler (`gcc`)
- the `make` program to run Makefile
- the clang-format program (not required at all)
- `git` (for cloning)

> [!NOTE]
>
> Almost all of these tools are OS independent, but they can differ
> running on UNIX like or Windows machine.

## Usage

How to use this project:

1. clone it to your machine.
```sh
git clone https://github.com/nasccped/citer-example && cd citer-example
```

2. build with make command:
```sh
make build

# Makefile doesn't work well in Windows OS, consider using the following command
mkdir out; gcc (Get-ChildItem -Recurse -Path src -Filter *.c | ForEach-Object { $_.FullName }) -o out/main.exe
```

3. run it with the binary path:
```sh
# expected bin path after running `make build`.
./out/main.exe --help
```

> [!NOTE]
>
> `make run` no longer works since the program expect `argc` to be
> greater than `1`.
>
> A basic run guide:
> - `<BIN_PATH> --help`: prints the project description
> - `<BIN_PATH> <1..>`: runs the project part
>
> Any other way of use isn't supported. The program will print an
> usage tip and quit with exit code `1`.

## Walkthrough

A few lessons are passed along the way.

1. **constructor & destructor:** how to create a new `CIterator` pointer
2. **set and create from:** how to set new data into `CIterator` + how
   create from the data itself
3. **cursor move:** how to move the cursor within an iterator queue
4. **getters:** using `CIterator` functions interface instead of
   manually field access.

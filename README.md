## Bitcode Retriever

Retrieves Bitcode from Mach-O binaries

### About

[Bitcode](http://llvm.org/docs/BitCodeFormat.html) stores as an [xar](https://en.wikipedia.org/wiki/Xar_(archiver)) archive inside of a Mach-O binary.

This tool extracts the archive and puts it near the binary, so it can be easily discovered using `xar` and `llvm-dis`

### Build

Clone the repo and run `make`, built binary lays inside of `build` directory

```bash
$ git clone git@github.com:AlexDenisov/bitcode_retriever.git
$ cd bitcode_retriever
$ make
```

### Usage

_Note: currently is does not work with static libraries, there is an opened issue #1, if you need this feature please a comment there, it will bump prioity of this project at my personal todo-list._

To use `bitcode_retriever` simple feed him your binary and it'll produce archive with bitcode.

It accepts both fat and non-fat binaries. For fat binaries it produces separate archive for each slice, e.g.:

```bash
$ bitcode_retriever fat_app
i386.xar
x86_64.xar
arm64.xar
```

for non-fat binaries it produces just one archive with the bitcode:

```bash
$ bitcode_retriever non_fat_app
i386.xar
```

To skip the xar archive and obtain the bitcode immediately, pass the `-e` argument.

```bash
$ bitcode_retriever -e fat_app
i386.1.bc
i386.2.bc
x86_64.1.bc
x86_64.2.bc
```

The project provides a sample binaries, you can play a bit with them:

```bash
$ make subject
$ cd build
$ ./bitcode_retriever i386.o
# or
$ ./bitcode_retriever fat.o
```

The xar archive stores set of files with bitcode:

```bash
$ xar -xf i386.o
$ ls
i386.o 1 2
```

You can dump LLVM IR from each file (`1`, `2`) using [`llvm-dis`](http://llvm.org/docs/CommandGuide/llvm-dis.html)

```bash
$ llvm-dis 1
$ llvm-dis 2
$ ls
1 2 1.ll 2.ll
```

### Bugs and issues

If you have any problems or found some bug - feel free to open an issue and/or send a pull request


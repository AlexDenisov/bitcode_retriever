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

To use `bitcode_retriever` simple feed him your binary and it'll produce archive with bitcode.

It accepts both fat and non-fat binaries. For fat binaries it produces separate archive for each slice, e.g.:

```bash
$ bitcode_retriever fat_app
$ ls
fat_app i386.xar x86_64.xar arm64.xar
```

for non-fat binaries it produces just one archive with the bitcode:

```bash
$ bitcode_retriever non_fat_app
$ ls
non_fat_app i386.xar
```

The project provides a sample binaries, you can play a bit with them:

```bash
$ make subject
$ cd build
$ ./bitcode_retriever i386.o
# or
$ ./bitcode_retriever fat.o
```

xar archive stores set of files with bitcode:

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


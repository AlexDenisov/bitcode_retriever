CC=/Applications/Xcode-Beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang
BUILD_DIR=build
BIN=$(BUILD_DIR)/bitcode_retriever

all:
	$(CC) main.c -o $(BIN)

clean:
	rm -rf $(BUILD_DIR)


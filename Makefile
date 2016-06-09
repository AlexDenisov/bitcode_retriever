CC=$(shell which clang)

BUILD_DIR=build
SUBJECT_DIR=subject

SOURCE_FILES=main.c macho_retriever.c macho_reader.c macho_util.c

RETRIEVER_BIN=$(BUILD_DIR)/bitcode_retriever

all: $(BUILD_DIR)
	$(CC) $(SOURCE_FILES) -o $(RETRIEVER_BIN) -lxar

### Testing

OBJECT_FILES=i386.o x86_64.o

subject: $(BUILD_DIR) fat.o
	@true

fat.o: $(OBJECT_FILES)
	cd $(BUILD_DIR) && lipo -create $^ -output $@

%.o:
	$(CC) -fembed-bitcode -c -arch $* $(SUBJECT_DIR)/main.c -o $(BUILD_DIR)/main.o
	$(CC) -fembed-bitcode -c -arch $* $(SUBJECT_DIR)/power2.c -o $(BUILD_DIR)/power2.o
	$(CC) -fembed-bitcode -arch $* $(BUILD_DIR)/main.o $(BUILD_DIR)/power2.o -o $(BUILD_DIR)/$@

$(BUILD_DIR):
	mkdir $@

clean:
	rm -rf $(BUILD_DIR)


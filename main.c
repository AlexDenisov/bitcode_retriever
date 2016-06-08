#include "macho_reader.h"
#include "macho_retriever.h"
#include "macho_util.h"

#include <string.h>

int main(int argc, char *argv[]) {
  int extract = 0;
  const char *filename;
  if (argc > 2) {
    extract = strcmp(argv[1], "-extract") == 0;
    filename = argv[2];
  } else {
    filename = argv[1];
  }

  FILE *stream = fopen(filename, "rb");

  struct bitcode_t *bitcodes[get_cpu_type_count()];
  int count;
  retrieve_bitcode(stream, bitcodes, &count);

  for (int i = 0; i < count; i++) {
    if (bitcodes[i]) {
      if (extract) {
        char *bitcode_files[1024];
        int bitcode_count;
        write_to_bitcode(bitcodes[i], bitcode_files, &bitcode_count);
      } else {
        write_to_xar(bitcodes[i]);
      }
    }
  }

  fclose(stream);
  return 0;
}

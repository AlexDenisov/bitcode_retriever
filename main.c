#include "macho_retriever.h"
#include "macho_reader.h"

#include <stdlib.h>

int main(int argc, char *argv[]) {
  const char *filename = argv[1];
  FILE *stream = fopen(filename, "rb");

  uint32_t magic = get_magic(stream, 0);
  int fat = is_fat(magic);
  int swap_bytes = is_should_swap_bytes(magic);

  if (fat) {
    retrieve_bitcode_from_fat(stream, swap_bytes);
  } else {
    retrieve_bitcode_from_nonfat(stream, 0);
  }

  fclose(stream);
  return 0;
}

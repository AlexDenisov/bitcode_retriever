#ifndef MACHO_READER_H
#define MACHO_READER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>

struct mach_header;
struct mach_header_64;
struct fat_header;

int get_cpu_type_count();
const char *get_cpu_type_name(struct mach_header *header);
const char *get_cpu_type_name_64(struct mach_header_64 *header);

uint32_t get_magic(FILE *stream, const int offset);

int is_magic_macho(const uint32_t magic);
int is_magic_64(const uint32_t magic);
int is_fat(const uint32_t magic);
int is_should_swap_bytes(const uint32_t magic);

struct fat_header *load_fat_header(FILE *stream, const int swap_bytes);
uint32_t offset_for_arch(FILE *stream, const int index, const int swap_bytes);

struct mach_header *load_mach_header(FILE *stream, const int offset, const int swap_bytes);
struct mach_header_64 *load_mach_header_64(FILE *stream, const int offset, const int swap_bytes);

struct segment_command *load_llvm_segment_command(FILE *stream, struct mach_header *header, const int offset, const int swap_bytes);
struct segment_command_64 *load_llvm_segment_command_64(FILE *stream, struct mach_header_64 *header, const int offset, const int swap_bytes);

#ifdef __cplusplus
}
#endif
#endif

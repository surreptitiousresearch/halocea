/* data_file_read_names @0x83809610 — reads the data file's name-table blob (the byte span between the
 * items and names cache-file offsets) from disk into a freshly allocated buffer. Reports a format error and
 * fails if the read is short. */

#include <stdint.h>
#include "headers/data_file_s.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern uint8_t cached_sync_read(void *file, void *buffer, int size, int offset);
extern int printf(const char *format, ...);

uint8_t data_file_read_names(data_file_s *data_file)
{
    unsigned int names_size = data_file->header.items_offset - data_file->header.names_offset;

    data_file->names = dlMalloc(names_size, "D:\\Projects\\code\\HCEX\\sources\\cache\\data_file.c", 0xACu);

    unsigned __int8 success = cached_sync_read(data_file->hFile, data_file->names, names_size, data_file->header.names_offset);

    if ( success )
    {
        data_file->cur_names_len = names_size;
        data_file->max_names_len = names_size;
    }
    else
    {
        printf("Invalid format in data file %s\n", data_file->file_name);
    }

    return success;
}

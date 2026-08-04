#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "headers/data_file_s.h"
#include "headers/blam_data_globals.h"
extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);

extern void *cache_files_open_cache_file(const char *path, uint8_t writable);
extern uint8_t data_file_read_header(data_file_s *data_file, uint32_t data_file_id);
extern uint8_t cached_sync_read(void *file, void *buffer, int size, int offset);
extern uint8_t data_file_read_items(data_file_s *data_file);
extern void dlFree(void *ptr);
extern int sprintf_0(char *string, const char *format, ...);
int    SetFilePointer(void *hFile, int distance, int *distance_high, uint32_t method);
int    WriteFile(void *hFile, const void *buf, uint32_t to_write, uint32_t *written, void *overlapped);

extern data_file_s  global_bitmap_data_file;

uint8_t data_file_open(uint32_t data_file_id, const char *file_name, uint8_t writable)
{
    int          result;
    data_file_s *data_file;
    const char  *dir;
    void        *hFile;
    int          names_size;
    char        *names;
    int          read_ok;
    char         header_valid;
    uint8_t      items_ok;
    uint8_t      success;
    uint32_t     bytes_written;
    char         path[320];

    result    = 0;
    data_file = 0;

    if (data_file_id == 1)
        data_file = &global_bitmap_data_file;
    else if (data_file_id == 2)
        data_file = &global_sound_data_file;

    memset(data_file, 0, sizeof(data_file_s));
    data_file->writable  = writable;
    data_file->file_name = file_name;

    dir = data_file_dir;
    if (!data_file_dir)
        dir = data_file_dir_default;

    sprintf_0(path, "%s\\%s.map", dir, file_name);
    hFile          = cache_files_open_cache_file(path, writable);
    data_file->hFile = hFile;

    if (hFile != (void *)-1)
    {
        if (data_file_read_header(data_file, data_file_id))
        {
            names_size   = data_file->header.items_offset - data_file->header.names_offset;
            names        = dlMalloc(names_size,
                               "D:\\Projects\\code\\HCEX\\sources\\cache\\data_file.c",
                               0xACu);
            data_file->names = names;
            read_ok = cached_sync_read(
                          data_file->hFile, names, names_size,
                          data_file->header.names_offset);
            if (read_ok)
            {
                data_file->cur_names_len = names_size;
                data_file->max_names_len = names_size;
                header_valid = 1;
                goto check_items;
            }
            printf("Invalid format in data file %s\n", data_file->file_name);
        }
        header_valid = 0;

check_items:
        items_ok = 0;
        if (header_valid)
        {
            items_ok = data_file_read_items(data_file) ? 1 : 0;
        }
        success = items_ok;
        result  = items_ok;

        if (success)
        {
            SetFilePointer(data_file->hFile, data_file->header.names_offset, 0, 0);
        }
        else
        {
            if (data_file->names)
            {
                dlFree(data_file->names);
                data_file->names = 0;
            }
            if (data_file->items)
            {
                dlFree(data_file->items);
                data_file->items = 0;
            }
        }

        if (!success && writable)
        {
            data_file->header.data_file_id  = data_file_id;
            data_file->header.names_offset  = 16;
            WriteFile(data_file->hFile, data_file, 0x10u, &bytes_written, 0);
            result = 1;
        }
    }

    if (!(uint8_t)result)
        printf("### FAILED TO OPEN DATA-CACHE FILE.\n\n");
    return result;
}

/* data_file_close @0x83809990 — close and reset one of the two known data-file caches (1 = global_bitmap_data_file,
 * 2 = global_sound_data_file — any other id leaves the pointer NULL and crashes on the handle close, as
 * shipped, no validation): closes the file handle, frees the names/items heap blocks if present, logs
 * hit/miss stats, and zeroes the header. Always returns 1. */

#include <stdint.h>
#include "headers/data_file_s.h"
#include "headers/win32_boundary.h"
#include "headers/blam_data_globals.h"


extern void dlFree(void *ptr);
/* printf provided by CRT via <stdio.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */

uint8_t data_file_close(unsigned int data_file_id)
{
    data_file_s *data_file = 0;

    if ( data_file_id == 1 )
        data_file = &global_bitmap_data_file;
    else if ( data_file_id == 2 )
        data_file = &global_sound_data_file;

    CloseHandle(data_file->hFile);

    if ( data_file->names )
        dlFree(data_file->names);
    if ( data_file->items )
        dlFree(data_file->items);

    printf("Cache pack file %s hits: %d for %3.2fM\n", data_file->file_name, data_file->hit_stats.items_size,
        (float)data_file->hit_stats.items_size * 0.00000095367432f);
    printf("Cache pack file %s adds/misses: %d for %3.2fM\n", data_file->file_name,
        data_file->miss_stats.items_size, (float)data_file->miss_stats.items_size * 0.00000095367432f);

    data_file->header.data_file_id = 0;
    data_file->header.names_offset = 0;
    data_file->header.items_offset = 0;
    data_file->header.item_count = 0;

    return 1;
}

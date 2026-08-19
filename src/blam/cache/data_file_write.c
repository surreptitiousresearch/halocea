/* data_file_write @0x83809AA0 — flush one of the two known data-file caches (1 = global_bitmap_data_file,
 * 2 = global_sound_data_file — any other id leaves the pointer NULL and crashes, as shipped) to disk, if
 * marked writable: writes the names blob, advances items_offset past it, optionally byte-swaps the item
 * records before writing them (data_file_reorder_on_write), then rewrites the 16-byte header at the start
 * of the file (also byte-swapped first, when reordering). */

#include "headers/data_file_s.h"
#include "headers/win32_boundary.h"
#include "headers/blam_data_globals.h"
#include "headers/byte_swap_definition.h"


extern void _byte_swap_memory(void *memory, int count, int code);

void data_file_write(unsigned int data_file_id)
{
    data_file_s *data_file = nullptr;

    if ( data_file_id == 1 )
        data_file = &global_bitmap_data_file;
    else if ( data_file_id == 2 )
        data_file = &global_sound_data_file;

    if ( !data_file->writable )
        return;

    unsigned int bytes_written;
    WriteFile(data_file->hFile, data_file->names, data_file->cur_names_len, &bytes_written, nullptr);

    data_file->header.items_offset = data_file->header.names_offset + data_file->cur_names_len;

    if ( data_file_reorder_on_write )
        _byte_swap_memory(data_file->items, (3 * data_file->header.item_count) & 0x3FFFFFFF, _4byte);

    WriteFile(data_file->hFile, data_file->items, 12 * data_file->header.item_count, &bytes_written, nullptr);

    if ( SetFilePointer(data_file->hFile, 0, nullptr, 0) != -1 )
    {
        if ( data_file_reorder_on_write )
            _byte_swap_memory(data_file, 4, _4byte);
        WriteFile(data_file->hFile, data_file, 0x10u, &bytes_written, nullptr);
    }
}

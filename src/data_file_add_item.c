/* data_file_add_item @0x83809C88 — add (or reuse) one named item in a writable build data file (id 1 = the
 * tags/maps build cache, id 2 = the sound build cache). If an item with the same name already exists and the
 * same data size, it is treated as a cache hit and its index returned; a same-name/different-size match is a
 * miss (returns -1). A brand-new name (when the file is writable) is appended: the item table and name buffer
 * grow geometrically as needed, the name is copied into the name buffer, the data is written to the file, and
 * the running data offset advances. Build hit/miss statistics are accumulated either way.
 *
 * The name length copied includes the terminating null (the strlen loop leaves the pointer one past it).
 * When the file is not writable, no item is appended and the function falls through to record a miss. */

#include <string.h>
#include "headers/data_file_s.h"
#include "headers/blam_data_globals.h"


extern int data_file_find_item(unsigned int data_file_id, const char *item_name);
extern void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line);
extern int WriteFile(void *file, const void *buffer, unsigned int count, unsigned int *written, void *overlapped);

int data_file_add_item(unsigned int data_file_id, const char *item_name, void *data, unsigned int data_size)
{
    data_file_s *file = nullptr;
    if ( data_file_id == 1 )
        file = &global_bitmap_data_file;
    else if ( data_file_id == 2 )
        file = &global_sound_data_file;

    int item_index = data_file_find_item(data_file_id, item_name);
    if ( item_index == -1 )
    {
        if ( file->writable )
        {
            if ( file->header.item_count >= file->max_item_count )
            {
                file->max_item_count += 16;
                file->items = dlRealloc(file->items, 12 * file->max_item_count,
                        "D:\\Projects\\code\\HCEX\\sources\\cache\\data_file.c", 0x190u);
            }

            unsigned int new_index = file->header.item_count;
            file->header.item_count = new_index + 1;
            item_index = new_index;
            file->items[new_index].data_size = data_size;
            file->items[new_index].data_offset = file->header.names_offset;

            const char *name_end = item_name;
            while ( *name_end++ )
                ;
            unsigned int name_length = name_end - item_name;   /* includes the terminating null */

            unsigned int names_len = file->cur_names_len;
            unsigned int needed_names_len = names_len + name_length;
            if ( needed_names_len >= file->max_names_len )
            {
                file->max_names_len = 2 * needed_names_len;
                file->names = dlRealloc(file->names, 2 * needed_names_len,
                        "D:\\Projects\\code\\HCEX\\sources\\cache\\data_file.c", 0x6Fu);
            }
            memcpy(&file->names[file->cur_names_len], item_name, name_length);
            file->cur_names_len += name_length;
            file->items[new_index].name_offset = names_len;

            unsigned int bytes_written;
            WriteFile(file->hFile, data, data_size, &bytes_written, nullptr);
            file->header.names_offset += data_size;
        }
    }
    else
    {
        if ( file->items[item_index].data_size == data_size )
        {
            ++file->hit_stats.items;
            file->hit_stats.items_size += data_size;
            return item_index;
        }
        item_index = -1;
    }

    ++file->miss_stats.items;
    file->miss_stats.items_size += data_size;
    return item_index;
}

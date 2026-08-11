/* data_file_append_name @0x838094A0 — append item_name (with its NUL terminator) to a data_file_s's name pool,
 * growing the pool via dlRealloc (doubling to 2x the new required length) when it would overflow
 * max_names_len. Returns the byte offset within the pool where item_name was appended (i.e. its offset
 * before this call). */

#include <string.h>
#include "headers/data_file_s.h"

extern void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line);

unsigned int data_file_append_name(data_file_s *data_file, const char *item_name)
{
    unsigned int name_offset = data_file->cur_names_len;
    unsigned int name_len = strlen(item_name) + 1;

    if ( data_file->cur_names_len + name_len >= data_file->max_names_len )
    {
        unsigned int new_max = 2 * (data_file->cur_names_len + name_len);
        data_file->max_names_len = new_max;
        data_file->names = dlRealloc(data_file->names, new_max,
            "D:\\Projects\\code\\HCEX\\sources\\cache\\data_file.c", 0x6Fu);
    }

    memcpy(&data_file->names[data_file->cur_names_len], item_name, name_len);
    data_file->cur_names_len += name_len;
    return name_offset;
}

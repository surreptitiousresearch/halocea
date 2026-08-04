#include "headers/data_file_s.h"

char * data_file_get_string(data_file_s *data_file, unsigned int index)
{
    if ( (int)index >= 0 && index < data_file->header.item_count )
        return &data_file->names[data_file->items[index].name_offset];
    return 0;
}

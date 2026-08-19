/* data_file_get_data_offset @0x83809E78 */
#include "headers/data_file_s.h"
#include "headers/blam_data_globals.h"


unsigned int data_file_get_data_offset(unsigned int data_file_id, unsigned int index)
{
    data_file_s *file = 0;

    if ( data_file_id == 1 )
        file = &global_bitmap_data_file;
    else if ( data_file_id == 2 )
        file = &global_sound_data_file;

    if ( (int)index >= 0 && index < file->header.item_count )
        return file->items[index].data_offset;
    return -1;
}

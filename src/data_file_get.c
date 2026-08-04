#include "headers/data_file_s.h"
#include "headers/blam_data_globals.h"


data_file_s * data_file_get(unsigned int data_file_id)
{
    if ( data_file_id == 1 )
        return &global_bitmap_data_file;
    if ( data_file_id == 2 )
        return &global_sound_data_file;
    return 0;
}

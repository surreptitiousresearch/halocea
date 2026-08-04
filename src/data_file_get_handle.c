#include <stdint.h>
#include "headers/data_file_s.h"
#include "headers/blam_data_globals.h"


void * data_file_get_handle(uint32_t data_file_id)
{
    if (data_file_id == 1)
        return global_bitmap_data_file.hFile;
    if (data_file_id == 2)
        return global_sound_data_file.hFile;
    /* id not 1 or 2: reads hFile (offset 0x3C) off a NULL data_file_s* — crashes as shipped, no validation */
    return ((data_file_s *)0)->hFile;
}

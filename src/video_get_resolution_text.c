#include <stdint.h>
#include "headers/video_resolution_s.h"

extern unsigned __int16 empty_wide_string;

uint16_t * video_get_resolution_text(int mode_index)
{
    if ( mode_index < 0 || mode_index >= video_resolution_count )
        return &empty_wide_string;
    return video_resolutions[mode_index].name;
}

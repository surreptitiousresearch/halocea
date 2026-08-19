/* video_resolution_find_refresh @0x83785190 — find `refresh`'s slot within mode_index's refresh-rate list,
 * or -1 if out of range / not found. */

#include "headers/video_resolution_s.h"

int video_resolution_find_refresh(int mode_index, unsigned int refresh)
{
    if ( mode_index < 0 || mode_index >= video_resolution_count )
        return -1;

    unsigned int num_refreshes = video_resolutions[mode_index].numRefreshes;

    if ( num_refreshes )
    {
        for ( unsigned int i = 0; i < num_refreshes; ++i )
        {
            if ( video_resolutions[mode_index].refreshes[i] == refresh )
                return i;
        }
    }
    return -1;
}

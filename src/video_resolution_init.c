/* video_resolution_init @0x837850CC — resets the video resolution table, then sorts the resolution list
 * (ascending by width/height) and each resolution's own refresh-rate list. */

#include <string.h>
#include <stdlib.h>
#include "headers/video_resolution_s.h"

extern int compare_video_resolutions(const video_resolution_s *res_1, const video_resolution_s *res_2);
extern int compare_video_refresh_rates(const unsigned int *ref_1, const unsigned int *ref_2);

void video_resolution_init(void)
{
    video_resolution_count = 0;
    memset(video_resolutions, 0, 32 * sizeof(video_resolution_s)); /* table holds 32 entries (disasm: memset size 0x980) */

    qsort(video_resolutions, 0, sizeof(video_resolution_s),
        (int (__fastcall *)(const void *, const void *))compare_video_resolutions);

    for ( int i = 0; i < video_resolution_count; ++i )
    {
        qsort(video_resolutions[i].refreshes, video_resolutions[i].numRefreshes, sizeof(unsigned int),
            (int (__fastcall *)(const void *, const void *))compare_video_refresh_rates);
    }
}

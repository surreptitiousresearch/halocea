/* video_resolution_add @0x83785508 — register (width, height) as a known video resolution if not already
 * present (capped at 32 entries), format its display name ("W x H"), and add `refresh` to its list of
 * known refresh rates (capped at 8, skipped if already present).
 *
 * DEVIATION: the decompiler's usprintf call shows only one variadic argument (width) against the
 * two-placeholder format "%d x %d"; disasm confirms r6 (height, already live from the preceding struct
 * store) is the second variadic argument — the compiler reused the register without an explicit move,
 * which the decompiler failed to recognize as a live call argument. */

#include <stdint.h>
#include "headers/video_resolution_s.h"

extern int video_resolution_find(unsigned int width, unsigned int height);
extern void usprintf(wchar_t *string, const wchar_t *format, ...);

void video_resolution_add(unsigned int width, unsigned int height, unsigned int refresh)
{
    int index = video_resolution_find(width, height);
    if ( index == -1 )
    {
        index = video_resolution_count;
        if ( video_resolution_count >= 32 )
            return;
        ++video_resolution_count;
    }

    video_resolutions[index].width = width;
    video_resolutions[index].height = height;
    usprintf(video_resolutions[index].name, L"%d x %d", width, height);
    video_resolutions[index].name[15] = 0;

    unsigned int refresh_count = video_resolutions[index].numRefreshes;
    uint8_t already_present = 0;

    if ( refresh_count )
    {
        for ( unsigned int i = 0; i < refresh_count; i++ )
        {
            if ( video_resolutions[index].refreshes[i] == refresh )
            {
                already_present = 1;
                break;
            }
        }
    }

    if ( !already_present && refresh_count < 8 )
    {
        video_resolutions[index].refreshes[refresh_count] = refresh;
        ++video_resolutions[index].numRefreshes;
    }
}

/* _rasterizer_windows_begin @ 0x8369BDE8
   Begins a render window. Bails if Halo rendering is disabled. If the device
   was lost it clears the flag and skips opening the scene this frame;
   otherwise it marks the scene as open. */

#include "headers/rasterizer_engine_globals.h"
#include "headers/blam_data_globals.h"


int _rasterizer_windows_begin(void)
{
    if ( hcex_off_halo_render )
        return 0;

    unsigned char was_lost = device_is_lost;
    if ( device_is_lost )
    {
        was_lost = 0;
        device_is_lost = 0;
    }
    if ( !was_lost )
        InsideScene = 1;
    return 1;
}

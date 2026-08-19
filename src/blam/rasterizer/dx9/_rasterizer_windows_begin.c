/* _rasterizer_windows_begin @ 0x8369BDE8
   Begins a render window. Bails if Halo rendering is disabled. If the device
   was lost it clears the flag and skips opening the scene this frame;
   otherwise it marks the scene as open. */

#include <stdint.h>
#include "headers/rasterizer_engine_globals.h"
#include "headers/blam_data_globals.h"

/* DEVIATION: every consumer byte-normalizes the return (clrlwi ..,r3,24 @0x8369BD24,
   @0x8370764C, @0x83707ABC) — declared return is the 8-bit Blam boolean, not int. */
uint8_t _rasterizer_windows_begin(void)
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

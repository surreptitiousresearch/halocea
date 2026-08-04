/* sound_permutation_get_real_mouth_aperture @ 0x837F8D68 — sample a sound permutation's per-tick
 * mouth-aperture data (one byte per tick, 0..255) at the given tick, clamped to range, and return
 * it normalized to [0,1]. Returns 0 if the permutation carries no mouth data. */

#include <stdint.h>
#include "headers/sound_permutation.h"

float sound_permutation_get_real_mouth_aperture(const sound_permutation *permutation, int16_t estimated_tick_index)
{
    int size = permutation->mouth_data.size;
    __int16 tick;

    if ( !size )
        return 0.0f;

    tick = estimated_tick_index;
    if ( estimated_tick_index >= 0 )
    {
        if ( estimated_tick_index > size - 1 )
            tick = (__int16)(size - 1);
    }
    else
    {
        tick = 0;
    }

    return (float)*((unsigned char *)permutation->mouth_data.address + tick) * 0.0039215689f; /* /255 */
}

/* _rasterizer_decals_dispose_from_old_map @0x83710F28 — internal per-map decal teardown: permanently
 * unlock decals and flush the decal vertex cache. (Public guard wrapper:
 * rasterizer_decals_dispose_from_old_map.) */

#include <stdint.h>
#include "headers/lruv_cache.h"
#include "headers/blam_data_globals.h"

extern void decals_unlock(uint8_t permanent);
extern void lruv_flush(lruv_cache *cache);

void _rasterizer_decals_dispose_from_old_map(void)
{
    if ( local_vertex_cache )
    {
        decals_unlock(1u);
        lruv_flush(local_vertex_cache);
    }
}

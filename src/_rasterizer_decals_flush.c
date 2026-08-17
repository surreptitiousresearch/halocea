/* _rasterizer_decals_flush @0x83710F70 */
#include <stdint.h>
#include "headers/blam_data_globals.h"
#include "headers/lruv_cache.h"
extern void decals_unlock(uint8_t permanent);
extern void lruv_flush(lruv_cache *cache);

void _rasterizer_decals_flush(void)
{
    decals_unlock(0);
    lruv_flush(local_vertex_cache);
}

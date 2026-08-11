/* decals_dispose_from_old_map @ 0x83740538 — rasterizer decals + invalidate pool */
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
extern void rasterizer_decals_dispose_from_old_map(void);
extern void data_make_invalid(data_array *data);
void decals_dispose_from_old_map(void)
{
    rasterizer_decals_dispose_from_old_map();
    data_make_invalid(global_decal_data);
}

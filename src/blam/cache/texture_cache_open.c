/* texture_cache_open @0x836F3E08 — mark the texture cache's data array valid for use. */
#include "headers/pc_texture_cache_globals.h"
extern void data_make_valid(data_array *data);
void texture_cache_open(void)
{
    data_make_valid(pc_texture_cache_globals.textures);
}

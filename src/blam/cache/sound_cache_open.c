/* sound_cache_open @0x837E7440 — mark the sound cache's data array valid for use. */
#include "headers/pc_sound_cache_globals.h"
extern void data_make_valid(data_array *data);
void sound_cache_open(void)
{
    data_make_valid(pc_sound_cache_globals.cache_sounds);
}

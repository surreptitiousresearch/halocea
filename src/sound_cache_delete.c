/* sound_cache_delete @0x837E7308 */
#include "headers/pc_sound_cache_globals.h"

#include "headers/data_array.h"
extern void data_dispose(data_array *data);
extern void lruv_delete(lruv_cache *cache);

void sound_cache_delete(void)
{
    pc_sound_cache_globals.bInitialized = 0;
    data_dispose(pc_sound_cache_globals.cache_sounds);
    lruv_delete(pc_sound_cache_globals.cache);
}

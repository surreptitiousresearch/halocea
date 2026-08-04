/* scenario_tags_unload @0x836F6F00 — close the sound, texture, and tag caches and clear the loaded-tags state.
 *
 * Deviation: the decompiler nests these as cache_file_close(texture_cache_close(sound_cache_close())) via
 * leftover r3; all three are void no-arg calls — they are independent. */
extern void sound_cache_close(void);
extern void texture_cache_close(void);
extern void cache_file_close(void);
#include "headers/cache_file_tags_globals.h"
#include "headers/cache_file_tag_instance.h"
void scenario_tags_unload(void)
{
    sound_cache_close();
    texture_cache_close();
    cache_file_close();
    cache_file_globals.tags_loaded = 0;
    global_tag_instances = 0;
}

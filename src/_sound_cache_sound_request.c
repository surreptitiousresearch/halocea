/* _sound_cache_sound_request @0x837E7E60 — ensure a sound permutation is resident in the PC streaming
 * sound cache and (optionally) block until its data has been preloaded. If the permutation is not yet
 * cached and `load` is set, a new LRUV block sized to the (decompressed) sample data is allocated and a
 * cache-sound record is bound to it. Once a block exists the sound is preloaded via FMOD; when `block`
 * is set the routine spins (idling the sound mixer and yielding) until the load completes. Returns
 * non-zero once the sound is available; increments the record's reference count when `reference` is set. */

#include <stdint.h>
#include "headers/sound_permutation.h"
#include "headers/cache_sound_datum.h"
#include "headers/pc_sound_cache_globals.h"
#include "headers/data_array.h"
#include "headers/lruv_cache.h"

#include "headers/lruv_cache.h"
extern int lruv_block_new(lruv_cache *cache, int size);
extern void lruv_block_touch(lruv_cache *cache, uint16_t block_index);
extern int datum_new_at_index(data_array *data, int index);
extern void sound_cache_dump_to_file(void);
extern uint8_t preload_sound_fmod(sound_permutation *sound, uint8_t load);
extern uint32_t system_milliseconds(void);
extern int sound_render_time(void);
extern void sound_idle(void);
extern void         Sleep(unsigned int milliseconds);

uint8_t _sound_cache_sound_request(
        sound_permutation *sound,
        uint8_t            block,
        uint8_t            load,
        uint8_t            reference)
{
    int available = 0;

    if (sound->cache_block_index == -1 && load)
    {
        int size = 0;
        int compression = sound->duplicate_compression;
        if (compression == 1)
        {
            size = sound->samples.size;
            sound->unused0[0] = size;
        }
        else if (compression == 3 || !sound->duplicate_compression)
        {
            size = sound->samples.size;
        }

        int block_index = lruv_block_new(pc_sound_cache_globals.cache, size);
        if (block_index == -1)
        {
            sound_cache_dump_to_file();
            *(uint8_t *)0 = 1; /* deliberate null store: fatal on sound-cache exhaustion */
        }
        else
        {
            datum_new_at_index(pc_sound_cache_globals.cache_sounds, block_index);
            cache_sound_datum *record =
                &((cache_sound_datum *)pc_sound_cache_globals.cache_sounds->data)[(uint16_t)block_index]; /* DEVIATION: low-word extract — clrlslwi r11,r30,16,4 @0x837E7F00 fuses the mask into the x16 stride; the raw subscript dropped it */
            sound->cache_block_index = block_index;
            sound->cache_base_address = nullptr;
            record->sound = sound;
            record->request_index = 0;
            record->available = 1;
        }
    }

    int cache_block_index = sound->cache_block_index;
    if (cache_block_index != -1)
    {
        cache_sound_datum *record =
            &((cache_sound_datum *)pc_sound_cache_globals.cache_sounds->data)[(uint16_t)cache_block_index]; /* DEVIATION: same extract on the stored handle — clrlslwi r11,r11,16,4 @0x837E7F3C after lwz 0x2C(r31) */
        int loaded = preload_sound_fmod(sound, load);
        lruv_block_touch(pc_sound_cache_globals.cache, sound->cache_block_index);
        do
        {
            if (record->available)
            {
                if (!record->postprocessed)
                {
                    record->postprocessed = 1;
                    record->software_reference_count = 0;
                    record->hardware_reference_count = 0;
                }
                if ((uint8_t)loaded && reference)
                    ++record->software_reference_count;
                available = loaded;
            }
            else
            {
                unsigned int now = system_milliseconds();
                if (now - sound_render_time() > 0x84)
                    sound_idle();
                if (block)
                    Sleep(0);
            }
        }
        while (!(uint8_t)available && block);
    }

    return available;
}

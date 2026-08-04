#include "headers/pc_sound_cache_globals.h"
#include "headers/sound_permutation.h"
#include "headers/cache_sound_datum.h"
#include "headers/blam_data_globals.h"
#include "headers/data_array.h"


void sound_cache_sound_hardware_unlock(sound_permutation *sound)
{
    cache_sound_datum *entry = DATA_ARRAY_ELEMENT(pc_sound_cache_globals.cache_sounds, cache_sound_datum,
                                                  sound->cache_block_index);

    if ( entry->hardware_reference_count )
        --entry->hardware_reference_count;
    else
        ++assertion_count;
}

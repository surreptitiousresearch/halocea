#include "headers/sound_permutation.h"

void sound_cache_sound_new(int tag_index, sound_permutation *sound)
{
    sound->cache_tag_index   = tag_index;
    sound->cache_block_index = -1;
    sound->cache_base_address = 0;
}

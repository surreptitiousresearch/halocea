/* animation_is_compressed @0x837956E0 */
#include "headers/animation.h"
#include "headers/animation_flags.h"
#include "headers/blam_data_globals.h"


int animation_is_compressed(const animation *animation)  /* BOOL->int */
{
    return (animation->flags & (1u << _animation_compressed_bit)) != 0 && (hs_model_animation_compression_enabled || !animation->compressed_data_offset);
}

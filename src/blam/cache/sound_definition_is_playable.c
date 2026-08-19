/* sound_definition_is_playable @ 0x837154B8 — a sound can play only if it has at least one pitch
 * range with permutations, and its sound class is not disabled. */

#include <stdint.h>
#include "headers/sound_definition.h"
#include "headers/sound_pitch_range.h"
#include "headers/sound_class_definition.h"
#include "headers/global_tag_instances.h"

/* attested: callee byte-normalizes the return (clrlwi r3,24 @ 0x83715514) and the caller
 * re-normalizes (sound_new_impulse @ 0x83718718) => uint8_t. definition_index stays a
 * full-word tag handle; TAG_INSTANCE masks the low 16 bits (clrlslwi @ 0x837154BC). */
uint8_t sound_definition_is_playable(int definition_index)
{
    sound_definition *def = TAG_GET(sound_definition, definition_index);

    if ( !def->pitch_ranges.count )
        return 0;
    if ( !((sound_pitch_range *)def->pitch_ranges.address)[0].permutations.count )
        return 0;
    if ( sound_classes[def->class_index].disabled )
        return 0;
    return 1;
}

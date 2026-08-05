#pragma once
/* trigger_firing_effect — one element of weapon_trigger_definition.firing_effects ("barrel"):
   how many shots it serves and the per-firing-state effect/damage-effect references
   (132 bytes; layout from the database). effects/damage_effects are indexed by firing state
   (0 = firing, 1 = misfire/overcharged, 2 = empty). */

#include <stdint.h>
#include "tag_reference.h"

typedef struct trigger_firing_effect
{
    int16_t       shots_lower_bound;  /* 0x00 */
    int16_t       shots_upper_bound;  /* 0x02 */
    int           unused[8];          /* 0x04 */
    tag_reference effects[3];         /* 0x24 */
    tag_reference damage_effects[3];  /* 0x54 */
} trigger_firing_effect;              /* 132 bytes */

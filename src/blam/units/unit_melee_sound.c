/* unit_melee_sound @0x836CCAC0 — play the audio for a melee impact: the struck material's melee-hit sound
 * (looked up by material type), and the melee damage effect's own sound. Both play attached to the unit at the
 * global origin/forward with full scale. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/damage_effect_definition.h"
#include "headers/material_definition.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
extern material_definition *scenario_material_definition_get(int16_t material_type);
extern int object_impulse_sound_new(int object_index, int definition_index, int16_t node_index, const real_point3d *position, const real_vector3d *forward, float scale, uint8_t is_local_player);

void unit_melee_sound(int unit_index, int damage_definition_index, int16_t material_type)
{
    int material_sound_index = scenario_material_definition_get(material_type)->melee_hit_sound.index;
    if ( material_sound_index != -1 )
        object_impulse_sound_new(unit_index, material_sound_index, -1, global_origin3d, global_forward3d, 1.0f, 0);

    if ( damage_definition_index != -1 )
    {
        damage_effect_definition *damage_tag =
            TAG_GET(damage_effect_definition, damage_definition_index);
        int effect_sound_index = damage_tag->sound.index;
        if ( effect_sound_index != -1 )
            object_impulse_sound_new(unit_index, effect_sound_index, -1, global_origin3d, global_forward3d, 1.0f, 0);
    }
}

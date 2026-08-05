/* material_effect_new @0x836EA900 — spawn the impact effect (and impact sound) for a material being hit.
 * Looks up the effect for (effect_index, material_index) in the material-effects definition, offsets the
 * spawn point slightly along the surface normal, and spawns the unattached effect and/or impulse sound.
 * In HCEX the material is forced to hcex_material_effect_type when that override is set. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/material_effects_definition.h"
#include "headers/material_effect.h"
#include "headers/material_effect_material.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_rgb_color.h"
#include "headers/sound_source.h"   /* sound_location */
#include "headers/location.h"
#include "headers/effect_vector_field.h"
#include "headers/blam_data_globals.h"


#include "headers/effect_vector_field.h"
extern int effect_new_unattached_from_markers(int definition_index, int owner_object_index, const real_vector3d *translational_velocity, int16_t marker_count, const char **marker_names, real_point3d *marker_points, real_vector3d *marker_forwards, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field, uint8_t deterministic);
extern int unattached_impulse_sound_new(int definition_index, const sound_location *location, float scale, uint8_t is_player);

void material_effect_new(int effects_definition_index, int16_t effect_index, int16_t material_index,
        const real_point3d *origin, const real_vector3d *normal, const location *location, float scale,
        uint8_t is_player)
{
    material_effects_definition *definition =
        TAG_GET(material_effects_definition, effects_definition_index);
    material_effect *effect;
    material_effect_material *material;
    real_point3d spawn_point;

    if ( hcex_material_effect_type != -1 )
        material_index = hcex_material_effect_type;

    if ( effect_index >= definition->effects.count )
        return;

    effect = (material_effect *)definition->effects.address + effect_index;
    if ( material_index == -1 || material_index >= effect->materials.count )
        return;

    material = (material_effect_material *)effect->materials.address + material_index;
    spawn_point.n[0] = (normal->n[0] * 0.0099999998f) + origin->n[0];
    spawn_point.n[1] = ((normal->n[1] * 0.0099999998f) + origin->n[1]);
    spawn_point.n[2] = ((normal->n[2] * 0.0099999998f) + origin->n[2]);

    {
        int effect_tag = material->effect.index;
        if ( effect_tag != -1 )
            /* color arg is a fixed sentinel address in the decompiler output; impulse-field and
             * deterministic args are left uninitialized there. */
            effect_new_unattached_from_markers(effect_tag, -1, NULL, 1, NULL, &spawn_point,
                    (real_vector3d *)normal, /* marker_forwards is non-const (stored into markers.forwards) */
                    scale, 0.0, (const real_rgb_color *)0x82000000, NULL, 0);
    }

    {
        int sound_tag = material->sound.index;
        if ( sound_tag != -1 )
        {
            sound_location sound;

            /* DEVIATION: the decompiler fused unrelated word moves into cross-assigned temps
             * (location->leaf_index "into" forward.n[1], zero bits "into" game_location); disasm
             * 0x836EAA18-0x836EAA78 is four plain copies: position = spawn_point, forward = *normal
             * (lwz 0/4/8(r31)), velocity = *global_zero_vector3d, game_location = *location (the
             * 8-byte ld/std of r29). The previous rendering was a real transcription defect. */
            sound.position = spawn_point;
            sound.forward = *normal;
            sound.translational_velocity = *global_zero_vector3d;
            sound.game_location = *location;
            unattached_impulse_sound_new(sound_tag, &sound, scale, 0);
        }
    }
}

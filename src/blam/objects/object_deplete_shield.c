/* object_deplete_shield @0x836B2638 — drop an object's shield: if not already depleted (damage_flags
 * bit 8), spawn the collision model's shield_depleted_effect, zero current_shield_damage, set the
 * depleted flag, and reset the shield region permutations.
 *
 * Deviation: the effect_new_from_object color/impulse args are NULL (r9=r10=0, verified at 0x836B26CC). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/effect_vector_field.h"
#include "headers/global_tag_instances.h"
#include "headers/real_rgb_color.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/collision_model.h"
#include "headers/object_damage_flags.h"
#include "headers/blam_data_globals.h"

#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
extern int effect_new_from_object(int definition_index, int owner_object_index, int object_index, int16_t force_local_player_index, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field);
extern void object_permutation_shield_regions(int object_index, uint8_t active);

void object_deplete_shield(int object_index)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    if ( (object->object.damage_flags & (1u << _object_shield_depleted_bit)) != 0 )
        return;

    _object_definition *definition =
        TAG_GET(_object_definition, object->definition_index);
    int collision_model_index = definition->collision_model.index;
    if ( collision_model_index != -1 )
    {
        collision_model *collision = TAG_GET(collision_model, collision_model_index);
        effect_new_from_object(collision->resistance.shield_depleted_effect.index,
                               object_index, object_index, -1, 0.0, 0.0, 0, 0);
    }

    object->object.current_shield_damage = 0.0f;
    object->object.damage_flags |= (1u << _object_shield_depleted_bit);
    object_permutation_shield_regions(object_index, 0);
}

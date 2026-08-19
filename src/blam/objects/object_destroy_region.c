/* object_destroy_region @0x836B2E40 — destroy a single body region of an object (e.g. blow off a limb). If
 * the region isn't already destroyed (regions_destroyed_flags), spawn its destroyed_effect, permute the
 * model to the "~damaged" state, set the matching object damage_flags bits from the damage_region's flags,
 * optionally deplete the body, mark the region destroyed, and run the type-specific region-destroyed hook.
 *
 * Deviation: the effect_new_from_object color/impulse args are NULL float-skip phantoms (the decompiler's
 * 0x82000000 / 84*region_index values come from reserved GPR slots). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/effect_vector_field.h"
#include "headers/global_tag_instances.h"
#include "headers/real_rgb_color.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/collision_model.h"
#include "headers/damage_region.h"
#include "headers/object_damage_flags.h"
#include "headers/object_region_flags.h"
#include "headers/blam_data_globals.h"

#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
extern int effect_new_from_object(int definition_index, int owner_object_index, int object_index, int16_t force_local_player_index, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field);
extern void object_permute_region(int object_index, const char *permutation_name, int16_t desired_region_index, uint8_t active_flag);
extern void object_deplete_body(int object_index);
extern void object_type_handle_region_destroyed(int object_index, int region_index, unsigned int damage_region_flags);

void object_destroy_region(int object_index, int16_t region_index)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    _object_definition *definition =
        TAG_GET(_object_definition, object->definition_index);
    int resistance_tag = definition->collision_model.index;
    if ( resistance_tag == -1 )
        return;

    int region_bit = 1 << region_index;
    if ( (object->object.regions_destroyed_flags & region_bit) != 0 )
        return;

    collision_model *collision = TAG_GET(collision_model, resistance_tag);
    damage_region *region = &((damage_region *)collision->resistance.regions.address)[region_index];
    int region_flags = region->flags;

    effect_new_from_object(region->destroyed_effect.index, object_index, object_index, -1, 0.0, 0.0, 0, 0);
    object_permute_region(object_index, "~damaged", region_index, 1u);

    if ( (region_flags & (1u << _object_region_inhibits_melee_attack_bit)) != 0 )
        object->object.damage_flags |= (1u << _object_melee_attack_inhibited_bit);
    if ( (region_flags & (1u << _object_region_inhibits_ranged_attack_bit)) != 0 )
        object->object.damage_flags |= (1u << _object_ranged_attack_inhibited_bit);
    if ( (region_flags & (1u << _object_region_inhibits_walking_bit)) != 0 )
        object->object.damage_flags |= (1u << _object_walking_inhibited_bit);
    if ( (region_flags & (1u << _object_region_forces_drop_weapon_bit)) != 0 )
        object->object.damage_flags |= (1u << _object_cannot_hold_weapon_bit);
    if ( (region_flags & (1u << _object_region_forces_object_to_die_bit)) != 0 )
        object_deplete_body(object_index);

    object->object.regions_destroyed_flags |= region_bit;
    object_type_handle_region_destroyed(object_index, region_index, region_flags);
}

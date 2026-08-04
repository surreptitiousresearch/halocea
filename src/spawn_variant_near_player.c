/* spawn_variant_near_player @0x836C8DB0 — debug helper that spawns an actor variant (found by matching
 * variant_name against actor-variant tag names) as a unit just in front of the local player. It reads the
 * player's controlled unit, iterates the 'actv' tag group for the first variant whose name contains the query,
 * places a new object of that variant's unit type at a random heading 0.3 units from the player's origin,
 * settles a biped onto the ground, customizes the unit for the variant, and finally creates the controlling
 * actor (deleting the object if actor creation fails).
 *
 * DEVIATION: two call sites needed disasm to recover arguments the decompiler rendered as garbage/uninitialized:
 * biped_fix_position's trailing flags arrive in r8/r9/r10 per its own prologue (r7 is the dead GPR slot for
 * the float arg); this site sets li r8,1 / li r9,0 / li r10,0 = (fix_below=1, dont_teleport=0,
 * use_radius_as_multiplier=0) — corrected 2026-07-27 from an earlier r7-based reading that passed dont_teleport=1.
 * actor_create_for_unit's stack tail args are
 * initial_state = actor_default_state_alert, default_state = actor_default_state_none, initial_command_list_index=-1, noncombat_sequence_id=0. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/actor_definition.h"
#include "headers/actor_definition_flags.h"
#include "headers/player_datum.h"
#include "headers/data_array.h"
#include "headers/tag_iterator.h"
#include "headers/object_placement_data.h"
#include "headers/actor_variant_definition.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/actor_default_state.h"
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"


#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
extern int local_player_get_player_index(int16_t local_player_index);
extern void tag_iterator_new(tag_iterator *iterator, uint32_t key_group_tag);
extern int tag_iterator_next(tag_iterator *iterator);
extern char *tag_get_name(int16_t tag_index);
extern char *strstr(const char *haystack, const char *needle);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern real_vector3d *vector3d_from_angle(real_vector3d *vector, float angle);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern int object_new(object_placement_data *data);
extern uint8_t biped_fix_position(int biped_index, int line_of_site_object_index, const real_point3d *new_position, real_point3d *final_position, float max_radius_fudge_factor, uint8_t fix_below_new_position, uint8_t dont_teleport, uint8_t use_radius_as_multiplier);
extern void actor_customize_unit(int actor_variant_definition_index, int unit_index);
extern int actor_create_for_unit(uint8_t swarm, int unit_index, int actor_variant_definition_index, int encounter_index, int16_t squad_index, uint8_t allow_addition_to_other_squads, int disallow_actor_index, uint8_t initially_braindead, int16_t initial_state, int16_t default_state, int16_t initial_command_list_index, char noncombat_sequence_id);
extern void object_delete(int object_index);

void spawn_variant_near_player(const char *variant_name)
{
    int player_index = local_player_get_player_index(0);
    if ( player_index == -1 )
        return;

    int player_unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index;
    if ( player_unit_index == -1 )
        return;

    tag_iterator iterator;
    tag_iterator_new(&iterator, 0x61637476u);   /* 'actv' actor_variant group */
    int variant_index = tag_iterator_next(&iterator);
    if ( variant_index == -1 )
        return;

    while ( !strstr(tag_get_name(variant_index), variant_name) )
    {
        variant_index = tag_iterator_next(&iterator);
        if ( variant_index == -1 )
            return;
    }

    actor_variant_definition *variant_definition = TAG_GET(actor_variant_definition, variant_index);
    actor_definition *actor_def = TAG_GET(actor_definition, variant_definition->actor_reference.index);

    unsigned int *seed = get_global_random_seed_address();
    float heading = real_seed_random_range(seed, 0.0f, TWO_PI);

    object_placement_data placement;
    object_placement_data_new(&placement, variant_definition->unit_reference.index, -1);
    vector3d_from_angle(&placement.forward, heading);
    object_get_origin(player_unit_index, &placement.position);
    placement.position.n[0] = placement.forward.n[0] * 0.30000001f + placement.position.n[0];
    placement.position.n[1] = placement.forward.n[1] * 0.30000001f + placement.position.n[1];
    placement.position.n[2] = placement.forward.n[2] * 0.30000001f + (placement.position.n[2] + 0.30000001f);

    int new_object = object_new(&placement);
    if ( new_object == -1 )
        return;

    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, new_object)->datum;
    if ( !object->object.type )
        biped_fix_position(new_object, -1, &placement.position, nullptr, 1.0f, 1, 0, 0);

    actor_customize_unit(variant_index, new_object);
    if ( actor_create_for_unit((actor_def->flags & (1u << _actor_definition_swarm_actor_bit)) != 0, new_object, variant_index, -1, -1, 0,
            -1, 0, actor_default_state_alert, 0, -1, 0) == -1 )
    {
        object_delete(new_object);
    }
}

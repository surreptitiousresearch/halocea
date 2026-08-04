/* actors_spawn_from_unit @0x837217A8 — spawn `count` AI actors of a given actor-variant definition out of a
 * unit (e.g. an infection form bursting from a body / a swarm release). Each spawned biped is placed in a
 * random horizontal direction 0.3 world units from the unit's origin (and lifted 0.3 on z), settled onto the
 * floor (biped_fix_position), customised, and bound to a freshly created actor that inherits the source unit's
 * encounter and squad. When `velocity` > 0 the new biped is given a randomized outward launch impulse.
 *
 * Returns the number of actors successfully spawned.
 *
 * Deviation: `velocity` is a float (4th arg, f1); the decompiler renders it double. The trailing arguments of
 * biped_fix_position and actor_create_for_unit are float-GPR-skip phantoms the decompiler left uninitialized;
 * recovered from disasm: biped_fix_position(..., fix_below=1, dont_teleport=0, use_radius_as_multiplier=0) and
 * actor_create_for_unit(..., initial_state = actor_default_state_alert, default_state = actor_default_state_none, initial_command_list_index=-1,
 * noncombat_sequence_id=0). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_definition_flags.h"
#include "headers/object_placement_data.h"
#include "headers/real_vector3d.h"
#include "headers/unit_datum.h"
#include "headers/actor_datum.h"
#include "headers/actor_variant_definition.h"
#include "headers/actor_definition.h"
#include "headers/actor_default_state.h"
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"

#include "headers/real_point3d.h"
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern real_vector3d *vector3d_from_angle(real_vector3d *vector, float angle);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern int object_new(object_placement_data *data);
extern void object_delete(int object_index);
extern uint8_t biped_fix_position(int biped_index, int line_of_site_object_index, const real_point3d *new_position, real_point3d *final_position, float max_radius_fudge_factor, uint8_t fix_below_new_position, uint8_t dont_teleport, uint8_t use_radius_as_multiplier);
extern void biped_accelerate(int biped_index, real_vector3d *acceleration);
extern void actor_customize_unit(int actor_variant_definition_index, int unit_index);
extern int actor_create_for_unit(uint8_t swarm, int unit_index, int actor_variant_definition_index, int encounter_index, int16_t squad_index, uint8_t allow_addition_to_other_squads, int disallow_actor_index, uint8_t initially_braindead, int16_t initial_state, int16_t default_state, int16_t initial_command_list_index, char noncombat_sequence_id);

int actors_spawn_from_unit(int unit_index, int actor_variant_definition_index, int16_t count, float velocity)
{
    int spawned = 0;
    if ( actor_variant_definition_index == -1 || count <= 0 )
        return 0;

    unit_datum *source_unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);

    __int16 encounter_index;
    __int16 squad_index;
    if ( source_unit->unit.swarm_actor_index == -1 && source_unit->unit.actor_index == -1 )
    {
        encounter_index = source_unit->unit.fake_encounter_index;
        squad_index = source_unit->unit.fake_squad_index;
    }
    else
    {
        actor_datum *source_actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, source_unit->unit.actor_index);
        squad_index = source_actor->meta.squad_index;
        encounter_index = source_actor->meta.encounter_index;
    }

    if ( encounter_index == -1 || squad_index == -1 )
        return 0;

    actor_variant_definition *variant_def =
        TAG_GET(actor_variant_definition, actor_variant_definition_index);
    actor_definition *actor_def = TAG_GET(actor_definition, variant_def->actor_reference.index);

    for ( __int16 i = 0; i < count; i = (__int16)(i + 1) )
    {
        float angle = real_seed_random_range(get_global_random_seed_address(), 0.0f, TWO_PI);

        object_placement_data placement;
        object_placement_data_new(&placement, variant_def->unit_reference.index, -1);
        vector3d_from_angle(&placement.forward, angle);
        object_get_origin(unit_index, &placement.position);
        placement.position.n[0] = (placement.forward.n[0] * 0.30000001f) + placement.position.n[0];
        placement.position.n[1] = (placement.forward.n[1] * 0.30000001f) + placement.position.n[1];
        placement.position.n[2] = (placement.forward.n[2] * 0.30000001f)
                                + (placement.position.n[2] + 0.30000001f);

        int object_index = object_new(&placement);
        if ( object_index == -1 )
            continue;

        unit_datum *new_unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);
        if ( !new_unit->object.type )
            biped_fix_position(object_index, -1, &placement.position, 0, 1.0f, 1, 0, 0);

        actor_customize_unit(actor_variant_definition_index, object_index);
        if ( actor_create_for_unit((actor_def->flags & (1u << _actor_definition_swarm_actor_bit)) != 0, object_index, actor_variant_definition_index,
                                   encounter_index, squad_index, 0, -1, 0,
                                   actor_default_state_alert, actor_default_state_none, -1, 0) == -1 )
        {
            object_delete(object_index);
        }
        else
        {
            if ( velocity > 0.0f )
            {
                float speed_factor = real_seed_random_range(get_global_random_seed_address(), 0.5f, 1.0f);
                real_vector3d launch;
                launch.n[2] = real_seed_random_range(get_global_random_seed_address(), 0.80000001f, 1.5f)
                            * velocity;
                launch.n[0] = (placement.forward.n[0] * speed_factor) * velocity;
                launch.n[1] = (placement.forward.n[1] * speed_factor) * velocity;
                if ( !new_unit->object.type )
                    biped_accelerate(object_index, &launch);
            }
            spawned = (__int16)(spawned + 1);
        }
    }

    return spawned;
}

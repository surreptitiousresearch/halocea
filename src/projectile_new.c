/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* projectile_new @0x8375B040 — finish initialising a freshly-created projectile object. Marks it active,
 * seeds its projectile-state fields (owner, ultimate parent, "no first contact" markers), and computes its
 * lifetime/detonation timers from the projectile definition: the arming/detonation countdown (a fixed rate,
 * or a random range between the definition's min/max, times the 30 Hz tick rate) and the timer-rate field,
 * both stored as reciprocals when >= 1 tick. Locates the definition's "cont" material index, applies the
 * definition's initial-velocity scale to the object's velocity, sets/clears the underwater flag from a
 * location test, and runs the angular-velocity / exported-function / deceleration passes. In a networked
 * game (client or server) it clears the client prediction bytes. Always returns 1.
 *
 * Projectile object and definition fields are raw offsets in the projectile-handler corpus style (sibling of
 * projectile_new_from_network). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/object_header_datum.h"
#include "headers/projectile_datum.h"
#include "headers/projectile_definition.h"
#include "headers/object_attachment_definition.h"
#include "headers/object_flags.h"
#include "headers/projectile_datum_flags.h"
#include "headers/projectile_definition_flags.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
extern int object_get_ultimate_parent(int object_index);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern uint8_t scenario_location_underwater(const location *location, const real_point3d *position, int16_t *optional_weather_palette_index);
extern void projectile_adjust_for_angular_velocity_change(int object_index);
extern void projectile_export_function_values(int projectile_index);
extern void projectile_calculate_deceleration(int projectile_index);
extern int16_t game_connection(void);


uint8_t projectile_new(int projectile_index)
{
    projectile_datum *projectile = ((projectile_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, projectile_index)->datum);
    int parent_object = projectile->object.owner_object_index;
    projectile_definition *definition = TAG_GET(projectile_definition, projectile->definition_index);

    projectile->object.flags |= (1u << _object_dynamic_lighting_recompute_bit);
    projectile->projectile.flags = (1u << _projectile_tracer_bit);
    projectile->projectile.replicate_detonation = 0;
    projectile->projectile.target_object_index = -1;
    /* faithful: int write of 0xFFFF at +560 sets action=0 (BE high half) and hit_material_type=-1 */
    projectile->projectile.action = 0;
    projectile->projectile.hit_material_type = 0xFFFF;
    projectile->projectile.ignore_object_index = object_get_ultimate_parent(parent_object);

    int definition_flags = definition->projectile.flags;
    float detonation_rate;
    if ( (definition_flags & (1u << _projectile_detonation_max_time_if_attached_bit)) != 0 )
        detonation_rate = definition->projectile.detonation_minimum_time * 30.0f;
    else if ( (definition_flags & (1u << _projectile_minimum_unattached_detonation_time)) != 0 )
        detonation_rate = definition->projectile.detonation_minimum_time * 30.0f;
    else
    {
        float minimum = definition->projectile.detonation_minimum_time;
        float maximum = definition->projectile.detonation_maximum_time;
        detonation_rate = real_seed_random_range(get_global_random_seed_address(), minimum, maximum) * 30.0f;
    }
    if ( detonation_rate >= 1.0f )
        projectile->projectile.detonation_timer_delta = 1.0f / detonation_rate;

    float timer_rate = definition->projectile.arming_time * 30.0f;
    if ( timer_rate >= 1.0f )
        projectile->projectile.arming_time_delta = 1.0f / timer_rate;

    projectile->projectile.tracer_attachment_index_index = -1;
    int material_count = definition->object.attachments.count;
    if ( material_count > 0 )
    {
        int material_index = 0;
        while ( ((object_attachment_definition *)definition->object.attachments.address)[material_index].type.group_tag != 0x636F6E74 /* "cont" */ )
        {
            material_index = (__int16)(material_index + 1);
            if ( material_index >= material_count )
                goto velocity;
        }
        projectile->projectile.tracer_attachment_index_index = material_index;
    }

velocity:
    {
        float velocity_scale = definition->projectile.initial_velocity;
        projectile->object.translational_velocity.n[0] = projectile->object.forward.n[0] * velocity_scale + projectile->object.translational_velocity.n[0];
        projectile->object.translational_velocity.n[1] = projectile->object.forward.n[1] * velocity_scale + projectile->object.translational_velocity.n[1];
        projectile->object.translational_velocity.n[2] = projectile->object.forward.n[2] * velocity_scale + projectile->object.translational_velocity.n[2];
    }

    unsigned __int8 underwater = scenario_location_underwater(&projectile->object.location,
            &projectile->object.bounding_sphere_center, nullptr);
    unsigned int flags = projectile->object.flags;
    if ( underwater )
        projectile->object.flags = flags | (1u << _object_wholly_under_media_bit);
    else
        projectile->object.flags = flags & ~(1u << _object_wholly_under_media_bit);

    projectile_adjust_for_angular_velocity_change(projectile_index);
    projectile_export_function_values(projectile_index);
    projectile_calculate_deceleration(projectile_index);
    projectile->object.flags |= (1u << _object_shadowless_bit) | (1u << _object_deleted_when_deactivated_bit);

    __int16 connection = game_connection();
    if ( connection == _game_connection_network_client || connection == _game_connection_network_server )
    {
        projectile->projectile.baseline_valid = 0;
        projectile->projectile.baseline_index = 0;
        projectile->projectile.message_index = 0;
        projectile->object.was_network_at_rest = 0;
    }
    return 1;
}

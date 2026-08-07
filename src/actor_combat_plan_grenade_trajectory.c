/* actor_combat_plan_grenade_trajectory @0x837B87A8 — solve and validate a grenade throw from the
 * actor's current position (actor->meta grenade-origin fields) toward grenade_target: builds the
 * trajectory via actor_combat_build_grenade_trajectory, then verifies the resulting ballistic arc has
 * clear line of fire via ai_test_ballistic_line_of_fire (ignoring the actor's own vehicle, if any, and
 * the object it was told to ignore). On success, records the target, prop/ignore indices, resulting aim
 * vector and speed into the actor's grenade-throw state block, and clears the "trajectory changed" flag
 * at +1697. Returns 1 on success, 0 if either step failed. trajectory_type and grenade_target_prop_index
 * are read at the tail (stored into actor state) but otherwise unused by this function's own logic
 * (trajectory_type isn't referenced at all).
 *
 * DEVIATION: full disasm register trace was required — the decompiler both (a) renders two of
 * actor_combat_build_grenade_trajectory's output pointers (arc_initial_velocity/arc_acceleration) as
 * genuinely-uninitialized locals, when disasm shows they're valid stack-buffer addresses computed just
 * like the trajectory's other (correctly-typed) output locals, and (b) drops grenade_ignore_object_index
 * from the ai_test_ballistic_line_of_fire call entirely, mis-substituting an unrelated stack address.
 * Reconstructed via full register trace of both call sites (stack-argument-overflow-count cross-checked
 * against each callee's known parameter count to resolve GPR/FPR slot assignment).
 *
 * DEVIATION: the earlier reading passed the actor-variant tag as desired_impact_point, grenade_target
 * (cast to float*) as optional_ballistic_fraction_min, and a `lob` argument — all three from assuming
 * `float velocity_max` consumes no parameter slot. It does, so slot 3 is r6 and slot 4 is r7: the call
 * passes grenade_target (r6 = r30) and NULL (`li r7, 0`); r5 is only scratch for `lfs f1, 0x190(r5)`. */

#include <stdint.h>
#include "headers/actor_variant_definition.h"
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


extern uint8_t actor_combat_build_grenade_trajectory(int16_t grenade_type, const real_point3d *grenade_origin, float velocity_max, const real_point3d *desired_impact_point, float *optional_ballistic_fraction_min, real_vector3d *aim_vector, float *aim_speed, float *arc_time, real_vector3d *arc_initial_velocity, float *arc_acceleration);
extern uint8_t ai_test_ballistic_line_of_fire(int actor_index, const real_point3d *origin, float arc_time, const real_vector3d *arc_initial_velocity, float arc_acceleration, int ignore_object_index, uint8_t ignore_vehicles);

uint8_t actor_combat_plan_grenade_trajectory(int actor_index, int16_t trajectory_type, const real_point3d *grenade_target, int grenade_target_prop_index, int grenade_ignore_object_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    const actor_variant_definition *variant_tag = TAG_GET(const actor_variant_definition, actor->meta.variant_definition_index);

    real_point3d grenade_origin = actor->input.position.head_position;

    int16_t grenade_type = (variant_tag)->grenade_combat.grenade_type;
    float velocity_max = (variant_tag)->grenade_combat.grenade_maximum_velocity;

    real_vector3d aim_vector;
    float aim_speed;
    float arc_time;
    real_vector3d arc_initial_velocity;
    float arc_acceleration;

    if ( !actor_combat_build_grenade_trajectory(grenade_type, &grenade_origin, velocity_max,
            grenade_target, nullptr, &aim_vector, &aim_speed, &arc_time,
            &arc_initial_velocity, &arc_acceleration) )
        return 0;

    uint8_t ignore_vehicles = (actor->input.vehicle_index != -1);
    if ( !ai_test_ballistic_line_of_fire(actor_index, &grenade_origin, arc_time, &arc_initial_velocity,
            arc_acceleration, grenade_ignore_object_index, ignore_vehicles) )
        return 0;

    /* recovered: raw offsets 1704.. -> actor->control grenade-throw plan state block */
    actor->control.grenade_current_target.x = grenade_target->n[0];
    actor->control.grenade_current_target.y = grenade_target->n[1];
    actor->control.grenade_current_target.z = grenade_target->n[2];
    actor->control.grenade_current_prop_index = grenade_target_prop_index;
    actor->control.grenade_current_ignore_object_index = grenade_ignore_object_index;
    actor->control.grenade_current_aim_vector.n[0] = aim_vector.n[0];
    actor->control.grenade_current_aim_vector.n[1] = aim_vector.n[1];
    actor->control.grenade_current_aim_vector.n[2] = aim_vector.n[2];
    actor->control.grenade_current_aim_speed = aim_speed;
    actor->control.grenade_current_lob = 0;

    return 1;
}

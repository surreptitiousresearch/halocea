/* actor_look_idle_new_major_direction @0x837FB694 — pick a new "major" idle look/aim direction for an
 * actor. Unless a minor variation was requested, it first tries to look at an interesting prop
 * (actor_look_idle_find_prop). Failing that it builds a random direction within the actor definition's
 * yaw/pitch limits (aiming vs looking limits chosen by major_is_aiming; free-facing allows a full PI yaw,
 * and the aiming case flattens to horizontal, falling back to global_forward3d if degenerate; the lower
 * pitch is halved when actor->input.vehicle_gunner is set). The chosen direction is written to the actor's
 * idle_major_direction slot, and when a look timer is successfully started, the major direction is latched.
 * Returns the "interesting direction" flag from the prop search.
 *
 * DEVIATION: FPR-shadow — actor_look_find_random_vector takes four float args (yaw/pitch min/max) that each
 * reserve a skipped GPR slot, so its result_vector pointer lands in r10 (= actor->control.idle_major_direction._opaque),
 * NOT the decompiler's phantom r6 (= has_minor_variation). The center_point is actor->input.position.head_position
 * (r3 = actor+0x120). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/actor_idle_looking.h"
#include "headers/direction_specification.h"
#include "headers/direction_specification_type.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"


#include "headers/direction_specification.h"
#include "headers/real_point3d.h"
extern uint8_t actor_look_idle_find_prop(int actor_index, uint8_t free_facing, uint8_t aim_at_prop, direction_specification *direction, uint8_t *interesting_prop);
extern float normalize3d(real_vector3d *v);
extern uint8_t actor_look_find_random_vector(const real_point3d *center_point, const real_vector3d *base_vector, uint8_t avoid_obstructions, float yaw_min, float yaw_max, float pitch_min, float pitch_max, real_vector3d *result_vector);
extern int actor_look_idle_timer(int actor_index, actor_idle_looking *looking_definition, int16_t timer_type, uint8_t interesting_direction);

uint8_t actor_look_idle_new_major_direction(int actor_index, actor_idle_looking *looking_definition,
                                                    const real_vector3d *base_vector, uint8_t free_facing,
                                                    uint8_t major_is_aiming,
                                                    uint8_t has_minor_variation)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    actor_definition *actor_def = TAG_GET(actor_definition, actor->meta.definition_index);

    uint8_t prop = 0;
    uint8_t interesting_prop = 0;
    uint8_t interesting_direction = 0;
    actor->control.idle_major_active = 0;

    if (!has_minor_variation)
    {
        prop = actor_look_idle_find_prop(actor_index, free_facing, major_is_aiming,
                                         &actor->control.idle_major_direction, &interesting_prop);
        interesting_direction = interesting_prop;
    }

    if (!prop)
    {
        real_vector3d working_vector;
        working_vector.n[0] = base_vector->n[0];
        working_vector.n[1] = base_vector->n[1];
        working_vector.n[2] = base_vector->n[2];

        float yaw_limit;
        float pitch_limit;
        if (major_is_aiming)
        {
            if (free_facing)
                yaw_limit = PI;
            else
                yaw_limit = (actor_def->looking.maximum_aiming_deviation.n[0] <= actor_def->looking.idle_aiming_deviation.n[0]) ? actor_def->looking.maximum_aiming_deviation.n[0] : actor_def->looking.idle_aiming_deviation.n[0];
            pitch_limit = (actor_def->looking.maximum_aiming_deviation.n[1] <= actor_def->looking.idle_aiming_deviation.n[1]) ? actor_def->looking.maximum_aiming_deviation.n[1] : actor_def->looking.idle_aiming_deviation.n[1];

            working_vector.n[2] = 0.0f;
            if (normalize3d(&working_vector) == 0.0f)
            {
                working_vector.n[0] = global_forward3d->n[0];
                working_vector.n[1] = global_forward3d->n[1];
                working_vector.n[2] = global_forward3d->n[2];
            }
        }
        else
        {
            yaw_limit = (actor_def->looking.maximum_looking_deviation.n[0] <= actor_def->looking.idle_looking_deviation.n[0]) ? actor_def->looking.maximum_looking_deviation.n[0] : actor_def->looking.idle_looking_deviation.n[0];
            pitch_limit = (actor_def->looking.maximum_looking_deviation.n[1] <= actor_def->looking.idle_looking_deviation.n[1]) ? actor_def->looking.maximum_looking_deviation.n[1] : actor_def->looking.idle_looking_deviation.n[1];
        }

        float pitch_min = -pitch_limit;
        if (actor->input.vehicle_gunner)
            pitch_min *= 0.5f;

        actor->control.idle_major_direction.type = _direction_specification_vector;
        prop = actor_look_find_random_vector(&actor->input.position.head_position, &working_vector, 1,
                                             -yaw_limit, yaw_limit, pitch_min, pitch_limit,
                                             &actor->control.idle_major_direction.___u1.vector);
    }

    if (!prop)
        return interesting_direction;

    int timer = actor_look_idle_timer(actor_index, looking_definition, (major_is_aiming == 0) + 1, interesting_direction);
    actor->control.idle_major_timer = timer;
    if (timer != 0)
    {
        actor->control.idle_major_is_aiming = major_is_aiming;
        actor->control.idle_major_active = 1;
    }
    return interesting_direction;
}

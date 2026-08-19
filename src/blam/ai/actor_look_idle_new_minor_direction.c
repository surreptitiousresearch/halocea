/* actor_look_idle_new_minor_direction @0x837FB898 — pick the next idle "minor" look direction for an
 * actor. It first tries to look at an interesting prop; failing that it samples a random look vector
 * within the actor definition's idle yaw/pitch bounds (using the tighter "minor" yaw limits, or the wider set when
 * the actor is not in look mode 3), records it as the new look target (direction type 4), and starts the
 * idle look timer. The timer result is latched and idle_minor_active is set on success.
 *
 * The actor definition's idle-looking floats live in its typed looking properties. */

#include <stdint.h>
#include "headers/actor_definition.h"
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_idle_looking.h"
#include "headers/direction_specification.h"
#include "headers/direction_specification_type.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/actor_mode.h"
#include "headers/idle_timer.h"
#include "headers/blam_data_globals.h"


#include "headers/direction_specification.h"
#include "headers/real_point3d.h"
extern uint8_t actor_look_idle_find_prop(int actor_index, uint8_t free_facing, uint8_t aim_at_prop, direction_specification *direction, uint8_t *interesting_prop);
extern uint8_t actor_look_find_random_vector(const real_point3d *center_point, const real_vector3d *base_vector, uint8_t avoid_obstructions, float yaw_min, float yaw_max, float pitch_min, float pitch_max, real_vector3d *result_vector);
extern int actor_look_idle_timer(int actor_index, actor_idle_looking *looking_definition, int16_t timer_type, uint8_t interesting_direction);

void actor_look_idle_new_minor_direction(int actor_index, actor_idle_looking *looking_definition,
                                         const real_vector3d *base_vector)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    unsigned char interesting_prop = 0;

    /* meta.definition_index (disasm: lwz 0x58) = the actor 'actr' tag, whose looking properties this reads
     * — NOT the variant tag despite the old local name. */
    actor_definition *actor_def = TAG_GET(actor_definition, actor->meta.definition_index);
    actor->control.idle_minor_active = 0;

    unsigned char interesting_direction;
    unsigned char have_direction =
        actor_look_idle_find_prop(actor_index, 0, 0, &actor->control.idle_minor_direction, &interesting_prop);

    if (!have_direction)
    {
        float yaw_bound   = (actor_def->looking.maximum_looking_deviation.yaw <= actor_def->looking.idle_looking_deviation.yaw)
                          ?  actor_def->looking.maximum_looking_deviation.yaw :  actor_def->looking.idle_looking_deviation.yaw;
        float pitch_bound = (actor_def->looking.maximum_looking_deviation.pitch <= actor_def->looking.idle_looking_deviation.pitch)
                          ?  actor_def->looking.maximum_looking_deviation.pitch :  actor_def->looking.idle_looking_deviation.pitch;

        /* combat mode (3) uses the combat look-delta angles at +188/+192; otherwise the noncombat set at +180/+184 */
        const float *minor_yaw = (actor->state.mode != _actor_mode_combat) ? actor_def->looking.noncombat_looking_delta_angles
                                                           : actor_def->looking.combat_looking_delta_angles;

        float yaw_min = -yaw_bound;
        if (-yaw_bound <= -minor_yaw[0])
            yaw_min = -minor_yaw[0];

        float yaw_max = yaw_bound;
        if (yaw_bound > minor_yaw[1])
            yaw_max = minor_yaw[1];

        real_vector3d random_vector;
        have_direction = actor_look_find_random_vector(&actor->input.position.head_position, base_vector, 0,
                                                       yaw_min, yaw_max, -pitch_bound, pitch_bound,
                                                       &random_vector);
        if (have_direction)
        {
            interesting_direction = 0;
            actor->control.idle_minor_direction.type = _direction_specification_vector;
            actor->control.idle_minor_direction.___u1.vector.n[0] = random_vector.n[0];
            actor->control.idle_minor_direction.___u1.vector.n[1] = random_vector.n[1];
            actor->control.idle_minor_direction.___u1.vector.n[2] = random_vector.n[2];
        }
        else
        {
            interesting_direction = interesting_prop;
        }
    }
    else
    {
        interesting_direction = interesting_prop;
    }

    if (have_direction)
    {
        int timer = actor_look_idle_timer(actor_index, looking_definition, _idle_timer_looking, interesting_direction);
        actor->control.idle_minor_timer = timer;
        if (timer)
            actor->control.idle_minor_active = 1;
    }
}

/* actor_aim_jump @0x837C7870 — let an actor steer/clamp a biped's jump velocity. Only acts when the
 * actor is not mounted (actor +344 == -1). Swarm actors defer to actor_type_swarm_aim_jump. A normal
 * actor that wants to jump (flag +1328) builds a desired jump velocity from its stored jump direction
 * (+1332,+1336) scaled by jump speed (+1340) with vertical component (+1344); unless leaping, the
 * velocity magnitude is clamped down to the supplied jump_magnitude. The wants-to-jump flag is always
 * cleared. Returns 1.
 *
 * DEVIATION: FPR-shadow — the float jump_magnitude (4th arg) reserves r6, so the real jump_velocity
 * pointer arrives in r7 (rendered as a phantom 6th param); the decompiler's r6 "jump_velocity" param is
 * the shadow and is unused. DB prototype is ground truth and is used here. The swarm call likewise
 * forwards jump_velocity (disasm `mr r6,r7`). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/real_vector3d.h"
#include "headers/actor_action.h"
#include "headers/charge_goal.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);

extern void actor_type_swarm_aim_jump(int actor_index, int unit_index, float jump_magnitude, real_vector3d *jump_velocity);

uint8_t actor_aim_jump(int actor_index, int unit_index, uint8_t leap, float jump_magnitude, real_vector3d *jump_velocity)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if (actor->input.vehicle_index == -1)
    {
        if (actor->meta.swarm) /* swarm actor */
        {
            actor_type_swarm_aim_jump(actor_index, unit_index, jump_magnitude, jump_velocity);
        }
        else if (actor->control.jumping_targeted) /* wants to jump */
        {
            if (actor->state.action == actor_action_charge && actor->state.action_data.___u0.charge.goal == charge_goal_melee_leaping)
                leap = 1;

            float direction_x = actor->control.jump_alignment_vector.n[0];
            float direction_y = actor->control.jump_alignment_vector.n[1];
            float speed       = actor->control.jump_target_horizontal_vel;
            float velocity_x = direction_x * speed;
            float velocity_y = direction_y * speed;
            float velocity_z = actor->control.jump_target_vertical_vel;
            jump_velocity->n[0] = velocity_x;
            jump_velocity->n[1] = velocity_y;
            jump_velocity->n[2] = velocity_z;

            float magnitude = __fsqrts(velocity_z * velocity_z
                                       + (velocity_x * velocity_x + velocity_y * velocity_y));
            if (!leap && magnitude > jump_magnitude)
            {
                float scale = jump_magnitude / magnitude;
                jump_velocity->n[0] = velocity_x * scale;
                jump_velocity->n[1] = velocity_y * scale;
                jump_velocity->n[2] = velocity_z * scale;
            }
        }
    }

    actor->control.jumping_targeted = 0;
    return 1;
}

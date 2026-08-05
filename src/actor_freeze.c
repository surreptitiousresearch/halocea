/* actor_freeze @0x8371E538 — drive an actor's unit(s) into a neutral "frozen" control pose this frame and mark
 * the actor frozen (actor+0x7 = 1). For a single-unit actor it delegates to actor_freeze_unit; for a swarm actor
 * (actor+0x6 != 0) it applies the same neutral control to every live unit in the swarm.
 *
 * DEVIATION: the decompiler gives actor_freeze a second parameter (a2) that it forwards to actor_freeze_unit. The
 * disassembly shows no caller sets r4 (e.g. actor_braindead @0x8372090C calls with only actor_index) and
 * actor_freeze_unit ignores its second parameter, so a2 is a ghost from an uninitialized register; this is a
 * single-parameter function. The swarm-unit control setup mirrors actor_freeze_unit's body. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/swarm_datum.h"
#include "headers/unit_control_data.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"
#include "headers/unit_desired_animation_state.h"

extern void *memset(void *dest, int value, unsigned int count);
extern void actor_freeze_unit(int actor_index);
extern void unit_get_facing_vector(int unit_index, real_vector3d *facing_vector);
extern void unit_get_aiming_vector(int unit_index, real_vector3d *aiming_vector);
extern void unit_get_looking_vector(int unit_index, real_vector3d *looking_vector);
extern void unit_control(int unit_index, const unit_control_data *control_data, int completed_client_update_id);
extern void unit_set_actively_controlled(int unit_index, uint8_t actively_controlled);

void actor_freeze(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( !actor->meta.swarm )   /* not a swarm actor */
    {
        actor_freeze_unit(actor->meta.unit_index);   /* attested 1-param: vestigial 2nd arg dropped (disasm: caller sets only r3) */
        actor->meta.frozen = 1;
        return;
    }

    int swarm_index = actor->meta.swarm_unit_index;   /* actor+0x28 */
    if ( swarm_index == -1 )
    {
        actor->meta.frozen = 1;
        return;
    }

    swarm_datum *swarm = DATA_ARRAY_ELEMENT(swarm_data, swarm_datum, swarm_index);
    for ( int i = 0; i < swarm->unit_count; i = (int16_t)(i + 1) )
    {
        int unit_index = swarm->unit_indices[i];

        unit_control_data control;
        memset(&control, 0, sizeof(control));
        control.animation_state = _unit_animation_state_alert;
        control.control_flags = 0;
        control.aiming_speed = 1;
        control.weapon_index = -1;
        control.grenade_index = -1;
        control.zoom_level = -1;
        control.throttle = *global_zero_vector3d;
        unit_get_facing_vector(unit_index, &control.facing_vector);
        unit_get_aiming_vector(unit_index, &control.aiming_vector);
        unit_get_looking_vector(unit_index, &control.looking_vector);
        unit_control(unit_index, &control, -1);
        unit_set_actively_controlled(unit_index, 0);
    }
    actor->meta.frozen = 1;
}

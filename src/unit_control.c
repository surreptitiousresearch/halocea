/* unit_control @0x836C6ED0 — applies a decoded control packet to a unit object: throttle, trigger,
 * aiming speed, weapon/grenade/zoom selections, control flags, and the looking/aiming/facing vectors,
 * plus the animation state. On the server it also latches a melee/action-in-progress hint and keeps a
 * copy of the raw control data. The completed-client-update id is recorded (or marked absent). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_control_data.h"
#include "headers/unit_datum.h"
#include "headers/game_connection.h"
#include "headers/unit_control_flags.h"
#include "headers/blam_data_globals.h"

extern void *memcpy(void *dst, const void *src, unsigned int size);
extern int16_t game_connection(void);

void unit_control(int unit_index, const unit_control_data *control_data, int completed_client_update_id)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);

    if (game_connection() == _game_connection_network_server)
    {
        unit->unit.force_local_update =
            (control_data->control_flags & (1u << _unit_control_throw_grenade_bit)) != 0 || (control_data->control_flags & (1u << _unit_control_weapon_primary_trigger_bit)) != 0;
        /* raw control-packet copy lives in the DB-anonymous 64-byte block at _unit_datum+644 */
        memcpy(&unit->unit.___u87.most_recent_control_data_from_network, control_data, 0x40u);
    }

    unit->unit.throttle.n[0] = control_data->throttle.n[0];
    unit->unit.throttle.n[1] = control_data->throttle.n[1];
    unit->unit.throttle.n[2] = control_data->throttle.n[2];
    unit->unit.primary_trigger = control_data->primary_trigger;
    unit->unit.aiming_speed = control_data->aiming_speed;

    int weapon_index = (uint16_t)control_data->weapon_index;
    if (weapon_index != 0xFFFF)
        unit->unit.desired_weapon_index = weapon_index;
    int grenade_index = (uint16_t)control_data->grenade_index;
    if (grenade_index != 0xFFFF)
        unit->unit.desired_grenade_index = grenade_index;

    unit->unit.desired_zoom_level = control_data->zoom_level;
    unit->unit.control_flags = control_data->control_flags;
    unit->unit.desired_looking_vector.n[0] = control_data->looking_vector.n[0];
    unit->unit.desired_looking_vector.n[1] = control_data->looking_vector.n[1];
    unit->unit.desired_looking_vector.n[2] = control_data->looking_vector.n[2];
    unit->unit.desired_aiming_vector.n[0] = control_data->aiming_vector.n[0];
    unit->unit.desired_aiming_vector.n[1] = control_data->aiming_vector.n[1];
    unit->unit.desired_aiming_vector.n[2] = control_data->aiming_vector.n[2];
    unit->unit.desired_facing_vector.n[0] = control_data->facing_vector.n[0];
    unit->unit.desired_facing_vector.n[1] = control_data->facing_vector.n[1];
    unit->unit.desired_facing_vector.n[2] = control_data->facing_vector.n[2];
    unit->unit.animation.desired_state = control_data->animation_state;

    if (completed_client_update_id == -1)
    {
        unit->unit.did_just_complete_client_update = 0;
    }
    else
    {
        unit->unit.last_completed_client_update_id = completed_client_update_id;
        unit->unit.did_just_complete_client_update = 1;
    }
}

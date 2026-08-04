/* update_remote_player_with_server_position @0x836A8C78 — move a remote player's unit to the authoritative
 * server position. If the correction is large (more than 1.1 world units) the unit's translational
 * velocity is zeroed and its "at rest invalid" flag set so it doesn't keep coasting, then the object is
 * translated to the new position. */

#include "headers/unit_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"

extern float __fsqrts(float x);
extern void gearbox_object_translate(int object_index, const real_point3d *new_position);

void update_remote_player_with_server_position(int unit_index, unit_datum *unit, real_point3d *position)
{
    float delta_z = position->n[2] - unit->object.position.n[2];
    float delta_x = position->n[0] - unit->object.position.n[0];
    float delta_y = position->n[1] - unit->object.position.n[1];

    if (__fsqrts(((delta_y * delta_y)
               + ((delta_x * delta_x) + (delta_z * delta_z)))) > 1.1f)
    {
        unsigned int flags = unit->object.flags | (1u << _object_at_rest_bit);
        unit->object.translational_velocity = *global_zero_vector3d;
        unit->object.flags = flags;
    }

    gearbox_object_translate(unit_index, position);
}

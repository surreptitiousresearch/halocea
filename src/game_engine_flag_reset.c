/* game_engine_flag_reset @0x83749E18 — returns a CTF flag (weapon object) to a reset position/orientation:
 * re-places it, resets its object state, clears the multiplayer-inventory flag (weapon flags bit 5), stamps
 * the reset time (last_owned_time), and clears its carrier (ignore_object_index, -1 = none). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_datum_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/real_vector3d.h"
extern void object_set_position(int object_index, const real_point3d *position, const real_vector3d *forward, const real_vector3d *up);
extern void object_reset(int object_index);
extern int game_time_get(void);

void game_engine_flag_reset(int weapon_index, const real_point3d *position)
{
    if ( weapon_index != -1 )
    {
        weapon_datum *weapon = ((weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum);

        object_set_position(weapon_index, position, global_forward3d, global_up3d);
        object_reset(weapon_index);

        weapon->weapon.flags &= ~(1u << _weapon_multiplayer_inventory_flag);
        weapon->item.last_owned_time = game_time_get();
        weapon->item.ignore_object_index = -1;
    }
}

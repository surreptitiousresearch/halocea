/* ctf_somebody_has_my_flag_in_vehicle @0x83806C40 — true if the enemy carrying `player`'s team's flag
 * (a weapon-type object, per ctf_globals.weapon_index[team]) is a passenger/driver in a vehicle.
 * Object internals resolved to the DB structs: flag holder = weapon_datum object.owner_player_index
 * (+192); unit's vehicle = unit_datum object.parent_object_index (+284). */

#include <stdint.h>
#include "headers/ctf_globals.h"
#include "headers/weapon_datum.h"
#include "headers/unit_datum.h"
#include "headers/player_datum.h"
#include "headers/data_array.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void *datum_try_and_get(const data_array *data, int index);

uint8_t ctf_somebody_has_my_flag_in_vehicle(player_datum *player)
{
    if (!player)
        return 0;

    int flag_weapon_index = ctf_globals.weapon_index[player->team_index];
    if (flag_weapon_index == -1)
        return 0;

    weapon_datum *flag_weapon = object_try_and_get_and_verify_type(flag_weapon_index, object_mask_weapon);
    if (!flag_weapon)
        return 0;

    int holder_player_index = flag_weapon->object.owner_player_index;   /* +192 */
    if (holder_player_index == -1)
        return 0;

    player_datum *holder = datum_try_and_get(player_data, holder_player_index);
    if (!holder)
        return 0;

    unit_datum *holder_unit = object_try_and_get_and_verify_type(holder->unit_index, object_mask_unit);
    if (!holder_unit)
        return 0;

    return holder_unit->object.parent_object_index != -1;   /* +284 */
}

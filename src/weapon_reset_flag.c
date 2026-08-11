/* weapon_reset_flag @0x838071E0 — if a weapon object is a placed CTF flag (its team index +184 selects a
 * ctf_globals.flags[] slot), clear that team's flag-warning state and, if the flag is still registered, reset
 * it to its scenario spawn position and clear its "at base"/mark its "flag returned" object flags.
 *
 * Object header lookup matches the established salted object-header idiom (see vehicle_reset.c). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/ctf_globals.h"
#include "headers/object_flags.h"
#include "headers/weapon_flags.h"
#include "headers/blam_data_globals.h"


extern void game_engine_flag_reset(int weapon_index, const real_point3d *position);

void weapon_reset_flag(int weapon_index)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    int16_t team_index = weapon->object.owner_team_index;

    ctf_globals.flag_warning[team_index] = 0;
    ctf_globals.flag_warning_time_in_ticks[team_index] = 0;

    scenario_netgame_flag *flag = ctf_globals.flags[team_index];
    if ( flag )
    {
        game_engine_flag_reset(weapon_index, &flag->position);
        weapon->object.flags |= (1u << _object_force_baseline_update_bit);
        weapon->weapon.flags &= ~(1u << _weapon_oddball_flag_has_been_touched_bit);
    }
}

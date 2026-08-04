/* create_the_flag @0x83805FB8 — spawns a team's CTF flag object at its scenario netgame flag placement, if
 * one exists for that team, and records the resulting object index. Automatic deactivation is disabled so
 * the flag object persists (dropped/carried) independent of the normal object lifetime rules. */

#include <stdint.h>
#include "headers/ctf_globals.h"
#include "headers/object_placement_data.h"

extern int get_flag_definition_index(void);
extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern int object_new(object_placement_data *data);
extern void object_set_automatic_deactivation(int object_index, uint8_t automatic_deactivation);

void create_the_flag(int team_index)
{
    scenario_netgame_flag *flag = ctf_globals.flags[team_index];

    if ( flag )
    {
        int flag_definition_index = get_flag_definition_index();
        object_placement_data placement;

        object_placement_data_new(&placement, flag_definition_index, -1);
        placement.owner_team_index = team_index;
        placement.position = flag->position;

        int object_index = object_new(&placement);
        object_set_automatic_deactivation(object_index, 0);

        if ( object_index != -1 )
            ctf_globals.weapon_index[team_index] = object_index;
    }
}

/* find_closest_vehicle @0x83829D60 — find the closest "vehicle spawn" (race vehicle) netgame flag to `point`,
 * skipping any flag index listed in `ignore_these`. If `point` is NULL, returns the index of the first
 * unignored vehicle-spawn flag found. Returns -1 if none qualify. */

#include "headers/scenario.h"
#include "headers/scenario_netgame_flag.h"
#include "headers/netgame_flag_type.h"
#include "headers/blam_data_globals.h"


int find_closest_vehicle(const real_point3d *point, int *ignore_these, int ignore_these_count)
{
    int closest_index = -1;
    int index = 0;
    float closest_distance_squared = 1000000.0f;

    int count = global_scenario->netgame_flags.count;
    if ( count > 0 )
    {
        const scenario_netgame_flag *flag = (const scenario_netgame_flag *)global_scenario->netgame_flags.address;
        do
        {
            if ( flag->type == _netgame_flag_race_vehicle )
            {
                unsigned __int8 ignored = 0;
                for ( int i = 0; i < ignore_these_count; i++ )
                {
                    if ( ignore_these[i] == index )
                    {
                        ignored = 1;
                        break;
                    }
                }

                if ( !ignored )
                {
                    if ( !point )
                        return index;

                    float dx = flag->position.n[0] - point->n[0];
                    float dy = flag->position.n[1] - point->n[1];
                    float dz = flag->position.n[2] - point->n[2];
                    float distance_squared = dx * dx + dy * dy + dz * dz;

                    if ( distance_squared < closest_distance_squared )
                    {
                        closest_distance_squared = distance_squared;
                        closest_index = index;
                    }
                }
            }

            ++index;
            ++flag;
        }
        while ( index < count );
    }

    return closest_index;
}

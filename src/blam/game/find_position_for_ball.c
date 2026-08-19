/* find_position_for_ball @0x83815EF0 — pick a spawn position for a ball object. If the active variant
 * isn't CTF-assault, first try the dedicated per-team "ball" netgame flag (type 2, team = ball_index)
 * via find_netgame_flag; if found, use its position directly. Otherwise (assault variant, or no dedicated
 * flag) fall back to picking uniformly at random among all netgame flags of type 2 ("ball" flags) in the
 * scenario. Leaves *result untouched (returns it unchanged) if no candidate flag exists at all.
 *
 * DEVIATION: the decompiler renders the find_netgame_flag call's type/team arguments as uninitialized
 * locals; disasm shows they're actually the constant 2 (type) and ball_index sign-extended (team). */

/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include <stdint.h>
#include "headers/blam_data_globals.h"
#include "headers/scenario.h"
#include "headers/scenario_netgame_flag.h"
#include "headers/netgame_flag_type.h"
#include "headers/game_variant.h"
#include "headers/real_point3d.h"


extern game_variant *game_engine_get_variant(void);
extern int find_netgame_flag(real_point3d *location, float distance, float height_delta, int16_t type, int16_t team);
extern uint32_t *get_global_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);

real_point3d * find_position_for_ball(real_point3d *result, int ball_index)
{
    scenario *scenario = global_scenario;
    const scenario_netgame_flag *flags = (const scenario_netgame_flag *)scenario->netgame_flags.address;
    int chosen_flag_index = -1;

    if ( !game_engine_get_variant()->game_engine_variant.ctf.assault )
        chosen_flag_index = find_netgame_flag(0, 0.0f, 0.0f, _netgame_flag_oddball_ball_spawn, (int16_t)ball_index);

    if ( chosen_flag_index == -1 )
    {
        int flag_count = scenario->netgame_flags.count;
        if ( flag_count <= 0 )
            return result;

        int ball_flag_count = 0;
        for ( int i = 0; i < flag_count; i = (int16_t)(i + 1) )
        {
            if ( flags[i].type == _netgame_flag_oddball_ball_spawn )
                ++ball_flag_count;
        }

        if ( !ball_flag_count )
            return result;

        unsigned int *random_seed = get_global_random_seed_address();
        int16_t skip_count = seed_random_range(random_seed, 0, ball_flag_count);

        flag_count = scenario->netgame_flags.count;
        if ( flag_count <= 0 )
            return result;

        int16_t i = 0;
        while ( 1 )
        {
            while ( flags[i].type != _netgame_flag_oddball_ball_spawn )
            {
                i = (int16_t)(i + 1);
                if ( i >= flag_count )
                    return result;
            }

            if ( !skip_count )
                break;
            --skip_count;
            i = (int16_t)(i + 1);
            if ( i >= flag_count )
                return result;
        }

        chosen_flag_index = i;
        if ( chosen_flag_index == -1 )
            return result;
    }

    *result = flags[chosen_flag_index].position;
    return result;
}

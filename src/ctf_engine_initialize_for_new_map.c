/* ctf_engine_initialize_for_new_map @0x83806098 — per-map initialization of the capture-the-flag engine.
 * Clears ctf_globals (and its baseline), then for each of the two teams resolves the team's netgame flag
 * marker (in assault mode the flag slots are swapped, since each team attacks the other's base). On a network
 * server it then either spawns both physical flag objects (classic CTF, single_flag_time <= 0) or, for
 * single-flag CTF, randomly assigns the flag to a team, announces the score, and starts the swap timer.
 * Finally, starting locations tagged for the wrong side of the map (closer to the enemy flag than their own,
 * or the reverse in assault) are reassigned to team 3.
 *
 * DEVIATION — dead-float-shadow-GPR: the DB decompile renders find_netgame_flag's type/team arguments as
 * uninitialised locals because the two float params (distance, height_delta) consume dead GPR shadows. Resolved
 * via disasm (0x83806110-0x83806124): type = li r6,0 = 0, team = extsh r7 = the loop team index.
 *
 * The score-clearing loop walks a pointer from ctf_globals.score up to &ctf_globals.score_to_win (the field
 * immediately after the two-element score array); reproduced verbatim. */

#include <stdint.h>
#include "headers/ctf_globals.h"
#include "headers/game_variant.h"
#include "headers/object_placement_data.h"
#include "headers/scenario.h"
#include "headers/scenario_player.h"
#include <string.h>
#include "headers/game_connection.h"
#include "headers/multiplayer_team.h"
#include "headers/ctf_message.h"
#include "headers/blam_data_globals.h"


extern game_variant *game_engine_get_variant(void);
extern int find_netgame_flag(real_point3d *location, float distance, float height_delta, __int16 type,
        __int16 team);
extern int16_t game_connection(void);
extern int get_flag_definition_index(void);
extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern int object_new(object_placement_data *data);
extern void object_set_automatic_deactivation(int object_index, uint8_t automatic_deactivation);
extern uint32_t *get_global_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern void create_the_flag(int team_index);
extern void game_show_score_team(int team, int message, unsigned __int8 should_replicate);
extern int16_t player_get_starting_location_count(void);
extern scenario_player * player_get_starting_location(int16_t location_index);
extern uint8_t match_game_type(int game_engine_type, int count, const int16_t *game_type);

unsigned __int8 ctf_engine_initialize_for_new_map(void)
{
    game_engine_get_variant();
    memset(&ctf_globals, 0, sizeof(ctf_globals));
    memset(&ctf_globals_baseline, 0, sizeof(ctf_globals_baseline));
    ctf_globals.weapon_index[0] = -1;
    ctf_globals.weapon_index[1] = -1;
    timeout_for_endgame_sound = 60;

    scenario *current_scenario = global_scenario;
    int *score = ctf_globals.score;
    int team = 0;
    do
    {
        int netgame_flag = find_netgame_flag(nullptr, 0.0f, 0.0f, 0, (__int16)team);
        *score = 0;
        int flag_slot = team;
        if ( game_engine_get_variant()->game_engine_variant.ctf.assault )
            flag_slot = (team + 1) % 2;
        ctf_globals.flags[flag_slot] = nullptr;
        if ( netgame_flag != -1 )
            ctf_globals.flags[flag_slot] =
                    &((scenario_netgame_flag *)current_scenario->netgame_flags.address)[netgame_flag];
        ++score;
        ++team;
    }
    while ( (int)score < (int)&ctf_globals.score_to_win );

    if ( game_connection() == _game_connection_network_server )
    {
        if ( game_engine_get_variant()->game_engine_variant.ctf.single_flag_time <= 0 )
        {
            /* classic CTF: spawn both teams' flag objects */
            for ( __int16 flag_team = 0; flag_team < 2; ++flag_team )
            {
                scenario_netgame_flag *flag = ctf_globals.flags[flag_team];
                if ( flag )
                {
                    object_placement_data placement;
                    object_placement_data_new(&placement, get_flag_definition_index(), -1);
                    placement.owner_team_index = flag_team;
                    placement.position.n[0] = flag->position.n[0];
                    placement.position.n[1] = flag->position.n[1];
                    placement.position.n[2] = flag->position.n[2];
                    int object_index = object_new(&placement);
                    object_set_automatic_deactivation(object_index, 0);
                    if ( object_index != -1 )
                        ctf_globals.weapon_index[flag_team] = object_index;
                }
            }
        }
        else
        {
            /* single-flag CTF: one flag, randomly assigned to a team */
            int team_with_flag = seed_random_range(get_global_random_seed_address(), 0, 2);
            create_the_flag(team_with_flag);
            ctf_globals.team_with_flag = team_with_flag;
            game_show_score_team(team_with_flag % 2, ctf_message_you_are_on_defense, 1u);
            game_show_score_team((team_with_flag + 1) % 2, ctf_message_you_are_on_attack, 1u);
            ctf_globals.flag_swap_timer = game_engine_get_variant()->game_engine_variant.ctf.single_flag_time;
        }
    }

    ctf_globals.score_to_win = game_engine_get_variant()->universal_variant.score_to_win;

    int starting_location_count = player_get_starting_location_count();
    for ( __int16 i = 0; i < starting_location_count; ++i )
    {
        scenario_player *location = player_get_starting_location(i);
        if ( (location->team_index && location->team_index != _multiplayer_team_blue)
          || !match_game_type(1, 4, location->game_type) )
            continue;

        unsigned __int8 assault = game_engine_get_variant()->game_engine_variant.ctf.assault != 0;
        int own_team = location->team_index % 2;
        scenario_netgame_flag *own_flag = ctf_globals.flags[own_team];
        scenario_netgame_flag *enemy_flag = ctf_globals.flags[(own_team + 1) % 2];

        float own_dz = (location->position.n[2] - own_flag->position.n[2]);
        float own_dy = (location->position.n[1] - own_flag->position.n[1]);
        float enemy_dz = (location->position.n[2] - enemy_flag->position.n[2]);
        float enemy_dy = (location->position.n[1] - enemy_flag->position.n[1]);
        float distance_to_own = ((own_dy * own_dy)
                + (((location->position.n[0] - own_flag->position.n[0])
                                * (location->position.n[0] - own_flag->position.n[0]))
                        + (own_dz * own_dz)));
        float distance_to_enemy = ((enemy_dy * enemy_dy)
                + (((location->position.n[0] - enemy_flag->position.n[0])
                                * (location->position.n[0] - enemy_flag->position.n[0]))
                        + (enemy_dz * enemy_dz)));

        unsigned __int8 reassign = 1;
        if ( assault )
        {
            if ( !(distance_to_own < distance_to_enemy) )
                reassign = 0;
        }
        else if ( !(distance_to_own > distance_to_enemy) )
        {
            reassign = 0;
        }

        if ( reassign )
            location->team_index = 3;
    }

    return 1;
}

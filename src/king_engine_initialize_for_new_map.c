/* king_engine_initialize_for_new_map @0x8382D2E0 — per-map init for the King-of-the-Hill game engine. Zeroes
 * the king globals and their network baseline, scans the scenario's netgame flags for hill markers
 * (_netgame_flag_hill),
 * collecting the set of distinct hill identifiers into king_engine_hills[] (with its count in king_engine_hill_count),
 * resets the live hill state (no hill selected, 1800-tick move timer, no previous controller), picks the
 * starting hill via find_hill, and initialises the default animation color/value tables to white/zero.
 *
 * The scenario netgame-flag records are read by raw offset (148-byte stride: +16 type, +18 identifier).
 * DEVIATION: find_hill is arg-less here — it reads king_globals.hill_id (which we set to 0 just above) and
 * global_scenario internally; the decompiler's `find_hill(v1, v3)` arguments are spurious register leftovers
 * (disasm at 0x8382D400 sets up no argument registers, and find_hill's entry at 0x8382BB70 loads king_globals
 * directly). hill_id/hill_state are genuinely 0 (the accumulated hill count lives in a different register). */

#include <stdint.h>
#include <string.h>
#include "headers/king_globals.h"
#include "headers/scenario.h"
#include "headers/scenario_netgame_flag.h"
#include "headers/netgame_flag_type.h"
#include "headers/king_hill_state.h"
#include "headers/real_rgb_color.h"
#include "headers/blam_data_globals.h"

extern int16_t king_engine_hill_count;

extern void find_hill(void);

uint8_t king_engine_initialize_for_new_map(void)
{
    scenario *current_scenario = global_scenario;

    memset(&king_globals, 0, sizeof(king_globals));
    memset(&king_globals_baseline, 0, sizeof(king_globals_baseline));

    king_engine_hill_count = 0;
    int16_t unique_hill_count = 0;

    for ( int flag_index = 0; flag_index < current_scenario->netgame_flags.count;
          flag_index = (int16_t)(flag_index + 1) )
    {
        scenario_netgame_flag *netgame_flag =
            &((scenario_netgame_flag *)current_scenario->netgame_flags.address)[flag_index];
        if ( netgame_flag->type == _netgame_flag_hill )
        {
            int16_t hill_identifier = netgame_flag->team_index; /* hill identifier reuses the team_index field */

            uint8_t already_collected = 0;
            for ( int i = 0; i < unique_hill_count; i = (int16_t)(i + 1) )
            {
                if ( king_engine_hills[i] == hill_identifier )
                {
                    already_collected = 1;
                    break;
                }
            }

            if ( !already_collected )
            {
                king_engine_hills[unique_hill_count] = hill_identifier;
                unique_hill_count = (int16_t)(unique_hill_count + 1);
            }
        }
    }
    king_engine_hill_count = unique_hill_count;

    king_globals.hill_id = 0;
    king_globals.hill_state = king_hill_uncontrolled;
    king_globals.hill_timer = 1800;
    king_globals.hill_previous_controller = -1;
    find_hill();

    global_default_animation_colors[0] = *global_real_rgb_white;
    global_default_animation_colors[1] = *global_real_rgb_white;
    global_default_animation_colors[2] = *global_real_rgb_white;
    global_default_animation_colors[3] = *global_real_rgb_white;
    global_default_animation_values[0] = 0.0f;
    global_default_animation_values[1] = 0.0f;
    global_default_animation_values[2] = 0.0f;
    global_default_animation_values[3] = 0.0f;
    return 1;
}

/* scenario_unload @0x837033D0 — unload the current scenario's tags and reset all scenario globals.
 *
 * Deviation: the decompiler renders a spurious `return &XBM__CaptureCompletionSignalMarker;` (tail-call ABI
 * noise after the void body); the function is void. */

#include "headers/scenario_globals.h"
#include "headers/game_globals.h"
#include "headers/blam_data_globals.h"


extern void scenario_tags_unload(void);

void scenario_unload(void)
{
    scenario_tags_unload();
    global_scenario_index = -1;
    global_structure_bsp_index = -1;
    scenario_globals->structure_bsp_index = -1;
    global_scenario = 0;
    global_structure_bsp = 0;
    global_collision_bsp = 0;
    global_bsp3d = 0;
    global_game_globals = 0;
}

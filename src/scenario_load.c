/* scenario_load @0x83704100 — load the named scenario's tags, resolve the scenario/globals tag pointers,
 * switch in the first structure BSP, and reset every netgame-equipment entry's spawn-object handle.
 * Returns 1 on success, 0 on any failure.
 *
 * Deviation: the failure path's loop over empty_string scans a buffer replacing '\n' with '\n' (a no-op as
 * decompiled — a Hex-Rays rendering of an assert/spew-string walk); reproduced verbatim. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_netgame_equipment.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"
/* DEVIATION: the decompiler modeled a 2-tag_block `scenario` {structure_bsp_references@0,
 * netgame_equipment@8}; the DB-verified scenario layout (scenario.h) places netgame_equipment@0x384
 * and structure_bsp_references@0x5A4 — matching disasm (`lwz 0x5A4`, `lwz 0x384`). Field names match,
 * so reusing the canonical header corrects the offsets. */

/* Removed `typedef struct game_globals game_globals;` — it collided with the canonical
 * data-global `game_globals` (struct_game_globals *). Use `struct game_globals` at the cast site. */

extern int scenario_tags_load(const char *name);
extern int tag_loaded(uint32_t group_tag, const char *name);
extern uint8_t scenario_switch_structure_bsp(int16_t structure_bsp_index);
extern char *strchr(const char *s, int c);
extern char empty_string[];

uint8_t scenario_load(const char *name)
{
    int scenario_index = scenario_tags_load(name);
    global_scenario_index = scenario_index;
    if ( scenario_index == -1 )
    {
        const char *cursor = empty_string;
        do
        {
            char *newline = strchr(cursor, 10);
            if ( !newline )
                break;
            cursor = newline + 1;
            *newline = 10;
        }
        while ( cursor );
        return 0;
    }

    global_scenario = TAG_GET(scenario, scenario_index);
    if ( global_scenario->structure_bsp_references.count <= 0 )
        return 0;

    global_game_globals = TAG_GET(struct game_globals, tag_loaded(0x6D617467u /* 'matg' */, "globals\\globals"));
    if ( !scenario_switch_structure_bsp(0) )
        return 0;

    scenario *scn = global_scenario;
    int index = 0;
    if ( global_scenario->netgame_equipment.count > 0 )
    {
        int offset = 0;
        do
        {
            ++index;
            char *equipment = (char *)scn->netgame_equipment.address + offset;
            offset += 144;
            ((scenario_netgame_equipment *)equipment)->run_time_spawned_item_index = -1;
            scn = global_scenario;
        }
        while ( index < global_scenario->netgame_equipment.count );
    }
    return 1;
}

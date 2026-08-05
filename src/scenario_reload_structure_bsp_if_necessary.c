/* scenario_reload_structure_bsp_if_necessary @0x83703C20 — if the scenario's desired structure BSP differs
 * from the loaded one, unload the current BSP reference and switch to the desired one. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/scenario_globals.h"
#include "headers/scenario_structure_bsp_reference.h"
#include "headers/breakable_surface_globals.h"

extern void scenario_structure_bsp_unload(const scenario_structure_bsp_reference *reference);
extern uint8_t scenario_switch_structure_bsp(int16_t structure_bsp_index);

/* DEVIATION: the decompiler threaded scenario_switch_structure_bsp's r3 out as a return value; the
 * binary leaves r3 undefined on the not-taken branch (beq @0x83703C54 -> epilogue, no li r3), so the
 * function is void. after_load_procs invokes it as a void-no-arg game_state_proc. */
void scenario_reload_structure_bsp_if_necessary(void)
{
    if ( scenario_globals->structure_bsp_index != global_structure_bsp_index )
    {
        scenario_structure_bsp_unload(
            (scenario_structure_bsp_reference *)global_scenario->structure_bsp_references.address + global_structure_bsp_index);
        global_structure_bsp_index = -1;
        scenario_switch_structure_bsp(scenario_globals->structure_bsp_index);
    }
}

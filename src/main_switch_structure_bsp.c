/* main_switch_structure_bsp @0x836892A0 — request a switch to a different structure BSP within the current
 * scenario, validating the index and rejecting a no-op switch to the current BSP. */

#include <stdint.h>
#include "headers/main_globals.h"
#include "headers/scenario.h"
#include "headers/blam_data_globals.h"

extern void console_warning(const char *format, ...);
extern void hud_load(uint8_t begin);

void main_switch_structure_bsp(int16_t new_structure_bsp_index)
{
    if ( new_structure_bsp_index < 0 || new_structure_bsp_index >= global_scenario->structure_bsp_references.count )
    {
        console_warning("tried to switch to invalid structure-bsp %d", new_structure_bsp_index);
    }
    else if ( new_structure_bsp_index == global_structure_bsp_index )
    {
        console_warning("tried to switch to current structure-bsp %d", new_structure_bsp_index);
    }
    else
    {
        main_globals.switch_to_structure_bsp_index = new_structure_bsp_index;
        hud_load(1u);
    }
}

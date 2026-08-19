/* main_switch_to_structure_bsp_private @0x8368A438 — perform a queued structure-BSP switch and reload the HUD. */

#include <stdint.h>
#include "headers/main_globals.h"

extern uint8_t scenario_switch_structure_bsp(int16_t structure_bsp_index);
extern void hud_load(uint8_t begin);

void main_switch_to_structure_bsp_private(void)
{
    scenario_switch_structure_bsp(main_globals.switch_to_structure_bsp_index);
    main_globals.switch_to_structure_bsp_index = -1;
    hud_load(0);
}

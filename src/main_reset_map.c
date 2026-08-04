/* main_reset_map @0x836890D0 — request the main loop to reset the current map (no BSP switch, no save, not a
 * lost-map condition). */

#include "headers/main_globals.h"

void main_reset_map(void)
{
    main_globals.switch_to_structure_bsp_index = -1;
    main_globals.save_map = 0;
    main_globals.reset_map = 1;
    main_globals.lost_map = 0;
}

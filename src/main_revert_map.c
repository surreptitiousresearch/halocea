/* main_revert_map @0x83689100 — request reverting to the last saved checkpoint. */

#include "headers/main_globals.h"

void main_revert_map(void)
{
    main_globals.switch_to_structure_bsp_index = -1;
    main_globals.save_map = 0;
    main_globals.revert_map = 1;
    main_globals.lost_map = 0;
}

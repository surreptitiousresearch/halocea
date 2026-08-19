/* main_skip_cinematic @0x83689130 — request that the running cinematic be skipped. */

#include "headers/main_globals.h"

void main_skip_cinematic(void)
{
    main_globals.switch_to_structure_bsp_index = -1;
    main_globals.save_map = 0;
    main_globals.skip_cinematic = 1;
}

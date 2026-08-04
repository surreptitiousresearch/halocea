#include <stdint.h>
#include "headers/blam_data_globals.h"
/* attract_mode_set_immediate_start @0x83784CC0 — setter for the immediate-start flag (see
 * attract_mode_should_start.c for the same global). */


void attract_mode_set_immediate_start(uint8_t start_immediately)
{
    attract_mode_immediate_start = start_immediately;
}

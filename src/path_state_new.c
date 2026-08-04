/* path_state_new @0x837D2490 — initializes a fresh path_state: zeroes the whole working set, anchors it
 * to the current structure BSP, copies the caller's path_input verbatim into the leading input field, and
 * stashes the optional debug-storage pointer. */

#include <string.h>

#include "headers/path_state.h"
#include "headers/structure_bsp.h"
#include "headers/blam_data_globals.h"


void path_state_new(const path_input *input, path_state *state, path_debug_storage *debug)
{
    memset(state, 0, sizeof(path_state));
    state->structure = global_structure_bsp;
    memcpy(&state->input, input, sizeof(path_input));
    state->debug = debug;
}

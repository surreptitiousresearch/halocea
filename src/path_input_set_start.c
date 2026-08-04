/* path_input_set_start @0x837D2230 — records the pathfinder's start point: marks it valid and copies
 * the point/surface index. Clean decompile, matches disasm_range(0x837D2230, 0x837D2254) directly. */

#include "headers/path_input.h"

void path_input_set_start(path_input *input, const real_point3d *start_point, int start_surface_index)
{
    input->start_valid = 1;
    input->start_point = *start_point;
    input->start_surface_index = start_surface_index;
}

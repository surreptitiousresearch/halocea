/* path_input_new @0x837D21D8 — resets a path_input to defaults: clears it, sets the pathfinding radius
 * and broken-surface tolerance, records the source object to ignore, and marks "no target object to
 * ignore" (-1).
 *
 * DEVIATION: the decompiler fabricates a phantom 5th parameter `a5`, misattributing the constant -1
 * write at offset 0xC (`ignore_target_object_index`) to it, and shows the byte written at offset 4
 * (`ignore_broken_surfaces`) as sourced from a wrongly-named local. disasm_range(0x837D21D8, 0x837D2230)
 * shows only 4 real incoming values (r3, f1 [dead-shadow r4], r5, r6) and confirms offset 0xC is always
 * the literal constant -1, never a parameter. Matches the already-established 4-arg extern in
 * `actor_path_input_new.c`. */

#include <string.h>
#include <stdint.h>

#include "headers/path_input.h"

void path_input_new(path_input *input, float pathfinding_radius, uint8_t ignore_broken_surfaces,
        int ignore_source_object_index)
{
    memset(input, 0, sizeof(path_input));
    input->pathfinding_radius = pathfinding_radius;
    input->ignore_broken_surfaces = ignore_broken_surfaces;
    input->ignore_source_object_index = ignore_source_object_index;
    input->ignore_target_object_index = -1;
}

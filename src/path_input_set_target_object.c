/* path_input_set_target_object @0x837D2228 — records the object the path is heading toward so the
 * pathfinder can ignore its collision (stored in path_input.ignore_target_object_index, mirroring
 * ignore_source_object_index for the pathing object itself). */

#include "headers/path_input.h"

void path_input_set_target_object(path_input *input, int target_object_index)
{
    input->ignore_target_object_index = target_object_index;
}

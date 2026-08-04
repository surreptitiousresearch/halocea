/* path_input_set_attractor @0x837D2258 — records a "soft" attraction point the pathfinder should be
 * biased toward within radius, weighted against the destination cost (mirrors path_input_set_target_object
 * for the harder ignore-collision case).
 *
 * DEVIATION: the decompiler shows 6 formal params with the 4th (object_index) unused and a phantom 6th
 * ("a6") storing to attractor_object_index. disasm_range(0x837D2258, 0x837D2298) shows the real mapping is
 * r3=input, r4=attractor_point, f1=radius (stored @0x38), r6=object_index (stored @0x34), f2=weight (stored
 * @0x3C) — r5/r7 are the dead GPR shadows PPC reserves alongside f1/f2. Reconstructed positionally from
 * disasm; matches the established 5-arg extern already used by actor_path_refresh. */

#include "headers/path_input.h"
#include "headers/real_point3d.h"

void path_input_set_attractor(path_input *input, const real_point3d *attractor_point, float radius,
        int object_index, float weight)
{
    input->attractor_valid = 1;
    input->attractor_point.n[0] = attractor_point->n[0];
    input->attractor_point.n[1] = attractor_point->n[1];
    input->attractor_point.n[2] = attractor_point->n[2];
    input->attractor_radius = radius;
    input->attractor_weight = weight;
    input->attractor_object_index = object_index;
}

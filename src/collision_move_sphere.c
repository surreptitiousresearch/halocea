/* collision_move_sphere @0x83775278 — sweeps a sphere by forwarding to collision_move_pill with a
 * zero-length pill (height=0.0, width=radius): a pill of zero height degenerates to a sphere.
 *
 * DEVIATION: the decompiler garbled this call — it dropped ignore_object_index entirely, rendered the
 * 0.0f height constant as a bogus int 0x82000000, and duplicated new_position's pointer cast in place of
 * new_velocity. Reconstructed from disasm: the PowerPC EABI here reserves a GPR slot for each FPR arg, so
 * pill's two float params (height, width) push its trailing int/pointer args one register higher than
 * sphere's — the visible mr-chain (r7->r8, r8->r9, r9->r10, r10->stack) performs exactly that shift. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_plane.h"

extern int16_t collision_move_pill(unsigned int flags, const real_point3d *old_position, const real_vector3d *old_velocity, float height, float width, int ignore_object_index, real_point3d *new_position, real_vector3d *new_velocity, int16_t maximum_collision_count, collision_plane *collisions);

__int16 collision_move_sphere(unsigned int flags, const real_point3d *old_position, const real_vector3d *old_velocity,
    float radius, int ignore_object_index, real_point3d *new_position, real_vector3d *new_velocity,
    __int16 maximum_collision_count, collision_plane *collisions)
{
    return collision_move_pill(flags, old_position, old_velocity, 0.0f, radius, ignore_object_index,
        new_position, new_velocity, maximum_collision_count, collisions);
}

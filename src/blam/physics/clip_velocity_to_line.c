/* clip_velocity_to_line @0x83774440 — projects velocity onto a line direction vector (dot(v,d)/dot(d,d) * d).
 * DEVIATION: the DB prototype lists a bogus 4th `result` parameter that the decompiler invented but never
 * reads or writes; disasm confirms only r3 (velocity), r4 (line_direction), r5 (result) are touched — the
 * decompiler's body writes into what it called `vector` (r5), which is the true output pointer. */

#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"

void clip_velocity_to_line(const real_vector3d *velocity, const real_point3d *line_direction, real_vector3d *result)
{
    float dot = velocity->n[0] * line_direction->n[0]
              + velocity->n[1] * line_direction->n[1]
              + velocity->n[2] * line_direction->n[2];
    float length_squared = line_direction->n[0] * line_direction->n[0]
                          + line_direction->n[1] * line_direction->n[1]
                          + line_direction->n[2] * line_direction->n[2];
    float scale = dot / length_squared;

    result->n[0] = line_direction->n[0] * scale;
    result->n[1] = line_direction->n[1] * scale;
    result->n[2] = line_direction->n[2] * scale;
}

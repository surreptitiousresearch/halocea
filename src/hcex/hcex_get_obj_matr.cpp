/* hcex_get_obj_matr @0x83683540 — fetch the world transform of a Blam object into an hcex_matr4x3,
 * but only report a change. Returns 0 (and leaves obj_matr untouched) when the object's current world
 * matrix is byte-identical to what obj_matr already holds; otherwise copies the new matrix in and
 * returns 1. Lets the caller skip redundant follower updates. */

#include <string.h>
#include "../headers/hcex/hcex_matr4x3.h"
#include "../headers/real_matrix4x3.h"

extern "C" real_matrix4x3 *object_get_world_matrix(int object_index, real_matrix4x3 *matrix);

extern "C" int hcex_get_obj_matr(int objId, hcex_matr4x3 *obj_matr)
{
    real_matrix4x3 world;

    object_get_world_matrix(objId, &world);
    /* deviation: original open-codes a per-byte compare loop; this is a memcmp over the 52-byte matrix */
    if ( memcmp(&world, obj_matr, sizeof(real_matrix4x3)) == 0 )
        return 0;
    memcpy(obj_matr, &world, sizeof(hcex_matr4x3));
    return 1;
}

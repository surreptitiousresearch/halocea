#include "headers/random_math_globals.h"
#include "headers/blam_data_globals.h"



real_vector3d *direction3d_from_table(real_vector3d *direction, __int16 index)
{
    *direction = random_math_globals.random_direction_table[index];
    return direction;
}

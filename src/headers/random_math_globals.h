#ifndef RANDOM_MATH_GLOBALS_H
#define RANDOM_MATH_GLOBALS_H
// DB-verified via types_members _6B8E6F75796320465CCB5AF25CBC93AA (anonymous PDB type)

#include <stdint.h>
#include "real_vector3d.h"

typedef struct random_math_globals_s
{
    real_vector3d *random_direction_table;
    int16_t        random_direction_table_size;
} random_math_globals_s;

#endif /* RANDOM_MATH_GLOBALS_H */

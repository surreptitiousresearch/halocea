#include <stdlib.h>
#include <stdint.h>
#include "headers/geosphere.h"
#include "headers/real_vector3d.h"
#include "headers/random_math_globals.h"
#include "headers/blam_data_globals.h"

extern uint32_t system_milliseconds(void);
extern uint32_t system_seconds(void);
extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern geosphere *geosphere_new(int16_t segment_count);
extern void geosphere_dispose(geosphere *sphere);


void random_math_initialize(void)
{
    uint32_t       seed;
    geosphere     *sphere;
    real_vector3d *directions;
    int            i;

    seed = system_milliseconds() ^ system_seconds();
    global_local_random_seed = rand() ^ seed;

    sphere     = geosphere_new(16);
    directions = dlMalloc(12 * sphere->vertex_count,
                     "D:\\Projects\\code\\HCEX\\sources\\math\\random_math.c", 0xB0u);
    /* canonical global is now typed random_math_globals_s; direct field access */
    random_math_globals.random_direction_table_size = sphere->vertex_count;
    random_math_globals.random_direction_table      = directions;

    for (i = 0; i < random_math_globals.random_direction_table_size; i = (int16_t)(i + 1))
    {
        directions[i].n[0] = sphere->vertices[i].n[0];
        directions[i].n[1] = sphere->vertices[i].n[1];
        directions[i].n[2] = sphere->vertices[i].n[2];
    }

    geosphere_dispose(sphere);
}

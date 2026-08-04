/* bsp2d_test_sphere_recursive @0x837E1830 — descend a leaf's 2D BSP with the projected query sphere, recursing
 * into whichever side(s) the radius-expanded sphere can reach and deferring to collision_surface_test_sphere at
 * leaf children. Faithful transcription (the front side is walked iteratively). */

#include "headers/test_sphere_data.h"
#include "headers/bsp2d_node.h"
#include "headers/real_point2d.h"

extern void collision_surface_test_sphere(test_sphere_data *data, int surface_index);

void bsp2d_test_sphere_recursive(test_sphere_data *data, int child_index)
{
    if ( child_index < 0 )
    {
        collision_surface_test_sphere(data, child_index & 0x7FFFFFFF);
        return;
    }

    const real_point2d *center2d = &data->center2d;
    while ( 1 )
    {
        float radius = data->radius;
        const float *node = (const float *)&((const bsp2d_node *)data->bsp->bsp2d.nodes.address)[child_index];
        float distance = (((node[1] * center2d->n[1]) + (center2d->n[0] * node[0]))
                               - node[2]);
        char reaches_front = (distance >= -radius);
        if ( distance <= radius )
            bsp2d_test_sphere_recursive(data, ((const int *)node)[3]);
        if ( !reaches_front )
            break;
        child_index = ((const int *)node)[4];
        if ( child_index < 0 )
        {
            collision_surface_test_sphere(data, child_index & 0x7FFFFFFF);
            break;
        }
    }
}

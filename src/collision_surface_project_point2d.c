/* collision_surface_project_point2d @0x837E0490 — lift a 2D point lying on a collision surface back to 3D by
 * projecting it onto the surface's supporting plane along the given axis/sign. The surface record stores the
 * index of its plane in the BSP's plane block. */

#include <stdint.h>
#include "headers/collision_bsp.h"
#include "headers/real_plane3d.h"
#include "headers/real_point2d.h"
#include "headers/real_point3d.h"

extern real_point3d *project_point2d(const real_point2d *p2d, const real_plane3d *plane, int16_t projection, uint8_t sign, real_point3d *p3d);

real_point3d *collision_surface_project_point2d(const collision_bsp *bsp, int surface_index,
                                                int16_t projection_axis, uint8_t projection_sign,
                                                const real_point2d *p2d, real_point3d *p3d)
{
    int plane_index = ((collision_surface *)bsp->surfaces.address)[surface_index].plane_designator;
    project_point2d(p2d, (const real_plane3d *)bsp->bsp3d.planes.address + plane_index, projection_axis,
                    projection_sign, p3d);
    return p3d;
}

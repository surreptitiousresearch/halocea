/* collision_surface_project_point2d @0x837E0490 — lift a 2D point lying on a collision surface back to 3D by
 * projecting it onto the surface's supporting plane along the given axis/sign. The surface record stores the
 * designator of its plane in the BSP's plane block.
 *
 * DEVIATION: plane_designator bit 31 is the facing flag, not index data. The binary never masks before
 * indexing because the 16-byte stride shift discards that bit in 32-bit arithmetic — `lwzx r11, r8, r4`
 * @0x837E04C4 loads the whole designator and `slwi r11, r11, 4` @0x837E04C8 shifts it straight into the
 * `add r4, r11, r10` @0x837E04CC that forms the plane pointer, so the shift IS the mask. This function
 * never reads the bit for anything else (it makes no facing test; the sign is project_point2d's
 * `projection_sign` argument, passed in separately). The `& 0x7FFFFFFF` below is therefore a proven no-op
 * on the as-built target and keeps the index in range where ptrdiff_t is wider (x64). Seventh member of the
 * UR-27/UR-27b designator-shift family, same fix shape as the other six. */

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
    int plane_designator = ((collision_surface *)bsp->surfaces.address)[surface_index].plane_designator;
    project_point2d(p2d, (const real_plane3d *)bsp->bsp3d.planes.address + (plane_designator & 0x7FFFFFFF),
                    projection_axis, projection_sign, p3d);
    return p3d;
}

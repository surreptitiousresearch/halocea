/* rasterizer_get_bitmap_default_hardware_format @0x8369B540 — return the default hardware texture format for a
 * bitmap's dimensionality: 2D (types 0 and 1) -> the 2D default, cube map (type 2) -> the cube default, 3D
 * (type 3) -> the white/3D default.
 *
 * DEVIATION: the original has no case for type > 3 and returns the uninitialized stack back-chain slot — an
 * indeterminate value. bitmap->type is always 0..3, so this path is dead; reproduced faithfully. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_type.h"
#include "headers/rasterizer_globals.h"

void *rasterizer_get_bitmap_default_hardware_format(const bitmap_data *bitmap)
{
    unsigned int type = (uint16_t)bitmap->type;
    if (type >= number_of_bitmap_types)
    {
        void *indeterminate; /* original returns the stack back-chain word here */
        return indeterminate;
    }
    if (type == bitmap_type_2d || type == bitmap_type_3d)
        return rasterizer_globals.default_2d_hardware_format;
    if (type == bitmap_type_cube_map)
        return rasterizer_globals.default_cm_hardware_format;
    return rasterizer_globals.default_white_hardware_format; /* bitmap_type_white */
}

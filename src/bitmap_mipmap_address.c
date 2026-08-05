/* bitmap_mipmap_address @0x836F45E8 — address of the top slice/face of a given mipmap level,
 * dispatching by bitmap dimensionality (2D / 3D / cube). Types >= 3 are invalid and return an
 * uninitialised stack value in the original (kept as a null-equivalent fallthrough here). */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_type.h"

extern char *bitmap_2d_address(const bitmap_data *bitmap, int16_t x, int16_t y, int16_t mipmap_index);
extern char *bitmap_3d_address(const bitmap_data *bitmap, int16_t x, int16_t y, int16_t z, int16_t mipmap_index);
extern void * bitmap_cube_map_address(const bitmap_data *bitmap, int16_t x, int16_t y, int16_t face_index, int16_t mipmap_index);

void * bitmap_mipmap_address(const bitmap_data *bitmap, int16_t mipmap_index)
{
    unsigned int type = (unsigned short)bitmap->type;

    if ( type == bitmap_type_2d )
        return bitmap_2d_address(bitmap, 0, 0, mipmap_index);
    if ( type == bitmap_type_3d )
        return bitmap_3d_address(bitmap, 0, 0, 0, mipmap_index);
    if ( type >= 3 )
        return 0;   /* invalid dimensionality (original returns uninitialised stack slot) */
    return bitmap_cube_map_address(bitmap, 0, 0, 0, mipmap_index);
}

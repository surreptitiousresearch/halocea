/* bitmap_vector_map @0x8377E028 — dispatches vector-map generation by bitmap type (0=2D, 1=3D, 2=cubemap). */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_type.h"

extern void bitmap_2d_vector_map(bitmap_data *bitmap);
extern void bitmap_3d_vector_map(bitmap_data *bitmap);
extern void bitmap_cm_vector_map(bitmap_data *bitmap);

void bitmap_vector_map(bitmap_data *bitmap)
{
    if (!bitmap->type)
        bitmap_2d_vector_map(bitmap);
    else if (bitmap->type == bitmap_type_3d)
        bitmap_3d_vector_map(bitmap);
    else if ((uint16_t)bitmap->type < bitmap_type_white)
        bitmap_cm_vector_map(bitmap);
}

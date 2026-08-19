/* bitmap_shrink @0x8377DB50 */
#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_type.h"

extern bitmap_data * bitmap_clone(const bitmap_data *source_bitmap);
extern bitmap_data * bitmap_2d_shrink(const bitmap_data *source_bitmap, int16_t scale, int16_t alpha_bias, uint8_t ignore_zero_alpha);
extern bitmap_data * bitmap_3d_shrink(const bitmap_data *source_bitmap, int16_t scale, int16_t alpha_bias, uint8_t ignore_zero_alpha);
extern bitmap_data * bitmap_cm_shrink(const bitmap_data *source_bitmap, int16_t scale, int16_t alpha_bias, uint8_t ignore_zero_alpha);

bitmap_data * bitmap_shrink(const bitmap_data *source_bitmap, int16_t scale, int16_t alpha_bias, uint8_t ignore_zero_alpha)
{
    if (scale <= 1)
        return bitmap_clone(source_bitmap);

    switch (source_bitmap->type)
    {
    case bitmap_type_2d:
        return bitmap_2d_shrink(source_bitmap, scale, alpha_bias, ignore_zero_alpha);
    case bitmap_type_3d:
        return bitmap_3d_shrink(source_bitmap, scale, alpha_bias, ignore_zero_alpha);
    case bitmap_type_cube_map:
        return bitmap_cm_shrink(source_bitmap, scale, alpha_bias, ignore_zero_alpha);
    default:
        return 0;
    }
}

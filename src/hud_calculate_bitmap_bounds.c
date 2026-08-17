/* hud_calculate_bitmap_bounds @0x8379E8D0 */
#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/real_rectangle2d.h"
#include "headers/hud_corner.h"

void hud_calculate_bitmap_bounds(
        const bitmap_data *bitmap,
        int16_t placement_type,
        const real_rectangle2d *clip,
        real_rectangle2d *bounds,
        uint8_t is_interface_bitmap)
{
    float w = (clip->n[1] - clip->n[0]) * (float)(is_interface_bitmap ? 1 : (int)bitmap->width);
    float h = (clip->n[3] - clip->n[2]) * (float)(is_interface_bitmap ? 1 : (int)bitmap->height);
    /* placement_type is a hud_corner anchor: each corner extends the box away from itself;
     * center (or anything >= 4) centers it. */
    switch (placement_type)
    {
    case _hud_corner_top_right:
        bounds->n[3] = h; bounds->n[0] = -w; bounds->n[1] = 0.0f; bounds->n[2] = 0.0f; break;
    case _hud_corner_bottom_left:
        bounds->n[2] = -h; bounds->n[1] = w; bounds->n[0] = 0.0f; bounds->n[3] = 0.0f; break;
    case _hud_corner_bottom_right:
        bounds->n[2] = -h; bounds->n[0] = -w; bounds->n[1] = 0.0f; bounds->n[3] = 0.0f; break;
    default:
        if (placement_type) /* _hud_corner_center and out-of-range */
        {
            bounds->n[0] = w * -0.5f; bounds->n[1] = w * 0.5f;
            bounds->n[2] = h * -0.5f; bounds->n[3] = h * 0.5f;
        }
        else /* _hud_corner_top_left */
        {
            bounds->n[3] = h; bounds->n[1] = w; bounds->n[0] = 0.0f; bounds->n[2] = 0.0f;
        }
        break;
    }
}

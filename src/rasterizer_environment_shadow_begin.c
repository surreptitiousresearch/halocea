/* rasterizer_environment_shadow_begin 0x836A61D8 — argument-forwarding tail-call thunk to
 * _rasterizer_environment_shadow_begin. */

#include <stdint.h>
#include "headers/real_matrix4x3.h"
#include "headers/real_rgb_color.h"

extern uint8_t _rasterizer_environment_shadow_begin(int object_index, const real_matrix4x3 *shadow_matrix, const real_rgb_color *shadow_color, float object_bounding_radius, float *shadow_volume_bounding_radius);

uint8_t rasterizer_environment_shadow_begin(
    int object_index, const real_matrix4x3 *shadow_matrix, const real_rgb_color *shadow_color,
    float object_bounding_radius, float *shadow_volume_bounding_radius)
{
    return _rasterizer_environment_shadow_begin(object_index, shadow_matrix, shadow_color,
                                                object_bounding_radius, shadow_volume_bounding_radius);
}

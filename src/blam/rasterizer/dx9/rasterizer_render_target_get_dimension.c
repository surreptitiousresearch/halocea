/* rasterizer_render_target_get_dimension @0x836F3520 — return the pixel width and height of one of
 * the global render targets, indexed by target. Straight table read of global_render_targets[target]. */

#include <stdint.h>
#include "headers/rasterizer_render_target.h"

void rasterizer_render_target_get_dimension(int16_t target, unsigned int *width, unsigned int *height)
{
    *width = global_render_targets[target].width;
    *height = global_render_targets[target].height;
}

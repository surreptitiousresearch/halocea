/* flag_set_trailing_shape @0x8380ED30 — stamp a flag's trailing-edge tesselation region according to its
 * definition's trailing_edge_shape: 0 = none (no-op). The split point is height-1 for shapes 3/4, else
 * height/2; the trailing block's X origin is clamped to >=0. Shape 1 first tesselates a "4" wedge across
 * the split, then a "3" (mirrored) rectangle below it; shape 2 first tesselates a "3" wedge, then a plain
 * "2" rectangle; shapes 3/4 tesselate a single region ("3" or "2" respectively) spanning the whole split. */

#include <stdint.h>
#include "headers/flag_definition.h"
#include "headers/flag_datum.h"
#include "headers/trailing_edge_shape.h"
#include "headers/tesselate.h"

extern void flag_tesselate_region(flag_definition *flag_definition, flag_datum *flag, int16_t x, int16_t y, int16_t size, int16_t tesselation);

void flag_set_trailing_shape(flag_definition *flag_definition, flag_datum *flag)
{
    int16_t trailing_edge_shape = flag_definition->trailing_edge_shape;
    if ( trailing_edge_shape == _trailing_edge_shape_flat )
        return;

    int16_t split;
    if ( trailing_edge_shape == _trailing_edge_shape_trapezoid_short_top
      || trailing_edge_shape == _trailing_edge_shape_trapezoid_short_bottom )
        split = flag_definition->height - 1;
    else
        split = flag_definition->height >> 1;

    int raw_x = flag_definition->width + flag_definition->trailing_edge_offset - split - 1;
    int16_t x = raw_x < 0 ? 0 : (int16_t)raw_x;

    int16_t y, size, tesselation;

    switch ( trailing_edge_shape )
    {
        case _trailing_edge_shape_trapezoid_short_top:
            tesselation = _tesselate_bottom_left;
            size = split;
            y = 0;
            break;

        case _trailing_edge_shape_trapezoid_short_bottom:
            size = split;
            y = 0;
            tesselation = _tesselate_top_left;
            break;

        case _trailing_edge_shape_concave_triangular:
            flag_tesselate_region(flag_definition, flag, x, 0, split, _tesselate_top_left);
            tesselation = 3;
            size = split;
            y = split;
            break;

        case _trailing_edge_shape_convex_triangular:
            flag_tesselate_region(flag_definition, flag, x, 0, split, _tesselate_bottom_left);
            size = split;
            y = split;
            tesselation = 2;
            break;

        default:
            return;
    }

    flag_tesselate_region(flag_definition, flag, x, y, size, tesselation);
}

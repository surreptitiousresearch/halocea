/* flag_set_attachment_shape @0x8380EBB0 — for a flag with attached_edge_shape set, stamp a triangular
 * tesselation wedge into flag->cells for every scripted attachment point, then tesselate that region
 * (tesselation level 5). Each attachment point (52-byte record, span at offset 0) contributes a span
 * clamped to [0, remaining rows]; within the resulting half-span x half-span block, cells are marked 4 on
 * the wedge's diagonal, 1 above it, 0 below it (in-bounds cells only). Advances the row cursor by the
 * even-rounded span after each point.
 *
 * DEVIATION: the decompiler's cell-value expression (`((__int16)(row-row_start) >> 15) +
 * ((__int16)(row-row_start) >= (unsigned int)(__int16)col) + ((unsigned int)(__int16)col >> 31)`) is a
 * compiled comparison-to-0/1 encoding; since `row >= row_start` and `col >= 0` always hold within this
 * loop's ranges, the sign-bit terms are always 0 and the expression reduces to a plain
 * `(row - row_start) >= col` comparison — verified by hand-tracing every term against the loop bounds. */

#include <stdint.h>
#include "headers/flag_definition.h"
#include "headers/flag_datum.h"
#include "headers/flag_attachment_point.h"
#include "headers/attached_edge_shape.h"

extern void flag_tesselate_region(flag_definition *flag_definition, flag_datum *flag, int16_t x, int16_t y, int16_t size, int16_t tesselation);

void flag_set_attachment_shape(flag_definition *flag_definition, flag_datum *flag)
{
    if ( flag_definition->attached_edge_shape == _attached_edge_shape_flat )
        return;

    int16_t point_index = 0;
    int16_t row_start = 0;

    while ( point_index < flag_definition->attachment_points.count )
    {
        int16_t height = flag_definition->height;
        if ( row_start >= height )
            break;

        int16_t span = ((flag_attachment_point *)flag_definition->attachment_points.address)[point_index].height_to_next_attachment;
        if ( span < 0 )
            span = 0;
        else if ( span > height - row_start )
            span = (int16_t)(height - row_start);

        int16_t even_span = (int16_t)(span & ~0x1); /* round span down to even */
        int16_t half_span = (int16_t)(even_span >> 1);

        if ( half_span > 0 )
        {
            int16_t row_end = (int16_t)(half_span + row_start);

            for ( int16_t col = 0; col < half_span; col = (int16_t)(col + 1) )
            {
                for ( int16_t row = row_start; row < row_end; row = (int16_t)(row + 1) )
                {
                    if ( col >= 0 && row >= 0 && col < flag_definition->width - 1 && row < flag_definition->height - 1 )
                    {
                        int16_t relative_row = (int16_t)(row - row_start);
                        int16_t tesselation = col == relative_row ? 4 : (relative_row >= col ? 1 : 0);
                        flag->cells[height * col + row].tesselation = tesselation;
                    }
                }
            }
        }

        flag_tesselate_region(flag_definition, flag, 0, (int16_t)(half_span + row_start), half_span, 5);

        point_index = (int16_t)(point_index + 1);
        row_start = (int16_t)(even_span + row_start);
    }
}

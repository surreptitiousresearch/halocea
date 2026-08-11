/* get_upper_bound_cell @0x83711938 — std::upper_bound counterpart of get_lower_bound_cell.c: returns the
 * first element strictly greater than `key`, over the same (cell_x, cell_y, cell_z) ordering. */

#include <stdint.h>
#include "headers/detail_object_cell_definition.h"
#include "headers/detail_object_cell_coord.h"

detail_object_cell_definition * get_upper_bound_cell(detail_object_cell_definition *begin, detail_object_cell_definition *end, detail_object_cell_coord *key)
{
    int count = (int)(end - begin);

    while ( count > 0 )
    {
        int half = count / 2;
        detail_object_cell_definition *mid = &begin[half];

        uint8_t mid_is_greater = mid->cell_x > key->x
            || (mid->cell_x == key->x && (mid->cell_y > key->y
                || (mid->cell_y == key->y && mid->cell_z > key->z)));

        if ( mid_is_greater )
        {
            count = half;
        }
        else
        {
            begin = mid + 1;
            count = count - half - 1;
        }
    }

    return begin;
}

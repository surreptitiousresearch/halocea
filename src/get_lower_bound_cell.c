/* get_lower_bound_cell @0x83711890 — std::lower_bound over a sorted [begin,end) run of
 * detail_object_cell_definition, ordered by (cell_x, cell_y, cell_z). Returns the first element not less than
 * `key`. */

#include <stdint.h>
#include "headers/detail_object_cell_definition.h"
#include "headers/detail_object_cell_coord.h"

detail_object_cell_definition * get_lower_bound_cell(detail_object_cell_definition *begin, detail_object_cell_definition *end, detail_object_cell_coord *key)
{
    int count = (int)(end - begin);

    while ( count > 0 )
    {
        int half = count / 2;
        detail_object_cell_definition *mid = &begin[half];

        uint8_t mid_is_less = mid->cell_x < key->x
            || (mid->cell_x == key->x && (mid->cell_y < key->y
                || (mid->cell_y == key->y && mid->cell_z < key->z)));

        if ( mid_is_less )
        {
            begin = mid + 1;
            count = count - half - 1;
        }
        else
        {
            count = half;
        }
    }

    return begin;
}

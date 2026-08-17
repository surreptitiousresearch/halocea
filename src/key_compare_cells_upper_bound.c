/* key_compare_cells_upper_bound @0x83711828 */
#include "headers/detail_object_cell_coord.h"
#include "headers/detail_object_cell_definition.h"

BOOL key_compare_cells_upper_bound(detail_object_cell_coord *key, detail_object_cell_definition *cell)
{
    if ( cell->cell_x <= key->x )
    {
        if ( cell->cell_x != key->x )
            return 0;
        if ( cell->cell_y <= key->y && (cell->cell_y != key->y || cell->cell_z <= key->z) )
            return 0;
    }
    return 1;
}

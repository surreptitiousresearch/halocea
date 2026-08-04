/* object_get_node_matrix @ 0x836EF090 — pointer to a single animated node matrix (by index) within an
 * object's node-matrix array. */

#include <stdint.h>
#include "headers/real_matrix4x3.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"


real_matrix4x3 *object_get_node_matrix(int object_index, int16_t node_index)
{
    object_header_datum *object_header = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    object_datum *object = object_header->datum;
    /* node matrices live in the header block region at the object's node_matrices block offset */
    return (real_matrix4x3 *)((char *)object + object->object.node_matrices.offset + 52 * node_index);
}

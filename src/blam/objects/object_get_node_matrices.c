/* object_get_node_matrices @ 0x836EBE68 — pointer to an object's array of animated node matrices. The
 * matrices live inside the object data at the byte offset stored in the node_matrices block reference. */

#include "headers/real_matrix4x3.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"


real_matrix4x3 *object_get_node_matrices(int object_index)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    return (real_matrix4x3 *)((char *)object + object->object.node_matrices.offset);
}

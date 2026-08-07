/* object_get_marker_by_name @ 0x836EF0D0 — resolve one or more of an object's markers by name into the
 * caller's object_marker array, with each marker's transform expressed in object space. Delegates to
 * model_get_marker_by_name (passing the object's model tag, its region permutation bytes, its node
 * matrices, and the mirror flag object.flags & 0x1000). If no named marker is found it falls back
 * to node 0: identity node matrix and the object's root node matrix as the object-space transform,
 * mirroring row 1 when the object is mirrored. Returns model_get_marker_by_name's count, or 1 for the
 * fallback when an empty name was requested. */

#include <stdint.h>
#include <string.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/object_marker.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"

extern int16_t model_get_marker_by_name(int model_index, const char *name, const uint8_t *region_permutations, const int16_t *node_remapping_table, int16_t node_count, const real_matrix4x3 *node_matrices, uint8_t mirrored_flag, object_marker *markers, int16_t maximum_marker_count);
extern void matrix4x3_identity(real_matrix4x3 *matrix);

int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count)
{
    object_header_datum *header =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    object_datum *object = header->datum;

    int16_t found_count = model_get_marker_by_name(
        TAG_GET(_object_definition, object->definition_index)->model.index,
        name,
        object->object.region_permutations,
        0,
        -1,
        (const real_matrix4x3 *)((char *)object + object->object.node_matrices.offset),
        (object->object.flags & (1u << _object_mirrored_bit)) != 0,
        markers,
        maximum_marker_count);
    if ( found_count )
        return found_count;

    /* Fallback: node 0, root node matrix in object space. */
    markers->node_index = 0;
    matrix4x3_identity(&markers->node_matrix);
    object_datum *object_again = header->datum;
    memcpy(&markers->matrix, (char *)object_again + object_again->object.node_matrices.offset,
        sizeof(markers->matrix));
    if ( (object->object.flags & (1u << _object_mirrored_bit)) != 0 )
    {
        markers->matrix.n[1][0] = -markers->matrix.n[1][0];
        markers->matrix.n[1][1] = -markers->matrix.n[1][1];
        markers->matrix.n[1][2] = -markers->matrix.n[1][2];
    }
    if ( !name )
        return found_count;
    if ( *name )
        return found_count;
    return 1;
}

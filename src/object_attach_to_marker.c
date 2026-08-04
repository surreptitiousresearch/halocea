/* object_attach_to_marker @0x836F1DB0 — position and orient a child object so that one of its markers
 * coincides with a marker on a parent object, then attach it to the parent's node.
 *
 * Both markers are resolved to their object-space transforms. If the child marker is named, the child is
 * aligned so its marker lands on the parent marker (object_align_marker_to_matrix). If the child marker
 * name is empty, the child's root is placed directly by transforming the parent marker's transform through
 * the inverse of the child marker's node matrix. Either way the child is reconnected to the map and bound
 * to the parent's node. */

#include <stdint.h>
#include "headers/object_datum.h"
#include "headers/object_marker.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"


#include "headers/location.h"
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern void object_disconnect_from_map(int object_index);
extern void object_align_marker_to_matrix(object_datum *object, const object_marker *child_marker, const real_matrix4x3 *destination_matrix);
extern void matrix4x3_inverse(const real_matrix4x3 *matrix, real_matrix4x3 *result);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern real_vector3d *matrix4x3_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);
extern void object_reconnect_to_map(int object_index, const location *location_in);
extern void object_attach_to_node(int parent_object_index, int child_object_index, int16_t parent_node_index);

void object_attach_to_marker(int parent_object_index, const char *parent_marker_name,
                             int child_object_index, const char *child_marker_name)
{
    object_datum *child = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, child_object_index)->datum;

    object_marker parent_marker;
    object_marker child_marker;
    object_get_marker_by_name(parent_object_index, parent_marker_name, &parent_marker, 1);
    object_get_marker_by_name(child_object_index, child_marker_name, &child_marker, 1);
    object_disconnect_from_map(child_object_index);

    if ( child_marker_name && *child_marker_name )
    {
        object_align_marker_to_matrix(child, &child_marker, &parent_marker.matrix);
    }
    else
    {
        /* place the child root directly: parent marker transform expressed in the child marker's frame.
         * The marker transform's rows are forward (n[0]), up (n[2]) and position (n[3]). */
        real_matrix4x3 child_node_inverse;
        matrix4x3_inverse(&child_marker.node_matrix, &child_node_inverse);
        matrix4x3_transform_point(&child_node_inverse, (const real_point3d *)parent_marker.matrix.n[3],
                                  &child->object.position);
        matrix4x3_transform_normal(&child_node_inverse, (const real_vector3d *)parent_marker.matrix.n[0],
                                   &child->object.forward);
        matrix4x3_transform_normal(&child_node_inverse, (const real_vector3d *)parent_marker.matrix.n[2],
                                   &child->object.up);
    }

    object_reconnect_to_map(child_object_index, NULL);
    object_attach_to_node(parent_object_index, child_object_index, parent_marker.node_index);
}

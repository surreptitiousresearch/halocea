/* object_attach_to_node @0x836F0AC0 — bind a child object to a specific node of a parent object.
 *
 * First it guards against cycles: it walks the parent's own parent chain (object data +284), and if the
 * child is found to already be an ancestor of the parent the attach is refused. Otherwise the child's
 * root transform is moved into the parent node's local space (inverse of the node matrix applied to the
 * child's position/forward/up), the child records its new parent object and node, and — if it had been
 * connected to the map — it is disconnected during the move and reconnected afterward. Finally the child
 * is deactivated, flagged do-not-update, and its node matrices recomputed.
 *
 * DEVIATION: two blocks of the decompiler output are verbatim compiler-inlined copies of named functions,
 * emitted here as the calls the source had: the parent fetch + node_matrices.offset + 52*node walk is
 * object_get_node_matrix @0x836EF090, and the tail header re-fetch + active-bit clear is
 * object_deactivate @0x836EBC40 (both already re-sourced; bodies match exactly). The decompiler's cached
 * data_array local (reloaded after the disconnect call) belonged to the first inlining; the single header
 * fetch serving both the deactivate test and the do-not-update OR is the inliner CSE'ing the callee's
 * fetch with the caller's. */

#include <stdint.h>
#include "headers/object_datum.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_header_flags.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/location.h"
extern void object_disconnect_from_map(int object_index);
extern void matrix4x3_inverse(const real_matrix4x3 *matrix, real_matrix4x3 *result);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern real_vector3d *matrix4x3_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);
extern void object_reconnect_to_map(int object_index, const location *location_in);
extern void object_compute_node_matrices(int object_index);
extern real_matrix4x3 *object_get_node_matrix(int object_index, int16_t node_index);
extern void object_deactivate(int object_index);

void object_attach_to_node(int parent_object_index, int child_object_index, int16_t parent_node_index)
{
    char can_attach = 1;

    /* cycle guard: if the child appears in the parent's ancestor chain, refuse the attach */
    int ancestor = parent_object_index;
    if ( parent_object_index != -1 )
    {
        while ( ancestor != child_object_index )
        {
            object_datum *ancestor_data =
                DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, ancestor)->datum;
            ancestor = ancestor_data->object.parent_object_index;
            if ( ancestor == -1 )
                goto attach;
        }
        can_attach = 0;
    }

attach:
    if ( !can_attach )
        return;

    object_datum *child_data =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, child_object_index)->datum;

    int was_connected = (child_data->object.flags >> _object_connected_to_map_bit) & 1;
    if ( was_connected )
        object_disconnect_from_map(child_object_index);

    real_matrix4x3 node_inverse;
    matrix4x3_inverse(object_get_node_matrix(parent_object_index, parent_node_index), &node_inverse);

    matrix4x3_transform_point(&node_inverse, &child_data->object.position, &child_data->object.position);
    matrix4x3_transform_normal(&node_inverse, &child_data->object.forward, &child_data->object.forward);
    matrix4x3_transform_normal(&node_inverse, &child_data->object.up, &child_data->object.up);

    child_data->object.parent_object_index = parent_object_index;
    child_data->object.parent_node_index = (char)parent_node_index;

    if ( was_connected )
        object_reconnect_to_map(child_object_index, NULL);

    object_deactivate(child_object_index);
    object_header_datum *child_header =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, child_object_index);
    child_header->flags |= (1u << _object_header_do_not_update_bit);

    object_compute_node_matrices(child_object_index);
}

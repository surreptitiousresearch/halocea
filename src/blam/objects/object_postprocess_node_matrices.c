/* object_postprocess_node_matrices @ 0x836ECF80 — after the node matrices are recomputed, give the
 * object's animation/model type a chance to fix them up, but only when the model tag has both a render
 * model (model.index != -1) and an animation graph (animation_graph.index != -1). The node-matrix array
 * lives inside the object data at the node_matrices block-reference offset. */

#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/real_matrix4x3.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/blam_data_globals.h"


extern void object_type_postprocess_node_matrices(int object_index, real_matrix4x3 *node_matrices);

void object_postprocess_node_matrices(int object_index)
{
    object_datum *object_data = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index))->datum;
    _object_definition *object_definition = TAG_GET(_object_definition, object_data->definition_index);

    if ( object_definition->model.index != -1 && object_definition->animation_graph.index != -1 )
        object_type_postprocess_node_matrices(object_index,
            (real_matrix4x3 *)((char *)object_data + object_data->object.node_matrices.offset));
}

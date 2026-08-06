/* hcex_get_obj_node_matrices @0x836835D0 — copy a Blam object's animated node matrices and region-permutation
 * indices out to the HCEX renderer. Resolves the object datum, follows its model tag (model index @
 * definition[13] == +52) to the model definition, and requires both to be valid. Copies node_count (model
 * definition) node matrices (52 bytes each == real_matrix4x3) from object_get_node_matrices() into
 * out_node_matrices, then copies the object's 8 region-permutation bytes into out_permutations.
 * Returns the node count (0 when the object has no valid model). */

#include <string.h>
#include "../headers/data_array.h"
#include "../headers/object_header_datum.h"
#include "../headers/object_datum.h"
#include "../headers/_object_definition.h"
#include "../headers/model.h"
#include "../headers/global_tag_instances.h"
#include "../headers/real_matrix4x3.h"
#include "../headers/hcex/hcex_matr4x3.h"

extern "C" data_array *object_header_data;
extern "C" real_matrix4x3 *object_get_node_matrices(int object_index);

extern "C" int hcex_get_obj_node_matrices(int objId, hcex_matr4x3 *out_node_matrices, unsigned char *out_permutations)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, objId)->datum;

    int model_index = (*(_object_definition **)TAG_INSTANCE(object->definition_index))->model.index;
    if ( model_index == -1 )
        return 0;

    model *model_definition = *(model **)TAG_INSTANCE(model_index);
    if ( !model_definition )
        return 0;

    int node_count = model_definition->nodes.count;
    memcpy(out_node_matrices, object_get_node_matrices(objId), 52 * node_count);

    for ( int i = 0; i < 8; ++i )
        out_permutations[i] = object->object.region_permutations[i];

    return node_count;
}

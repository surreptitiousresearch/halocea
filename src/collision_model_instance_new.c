/* collision_model_instance_new @ 0x837BB730 — bind an object's collision model and current node
 * matrices into a collision_model_instance for ray testing. Returns 0 if the object's definition has
 * no collision model (object def +124 == -1). The region/permutation indices live in the object data
 * at +384; node matrices come from the object's animation state.
 *
 * Object def +124 resolves to the DB-named _object_definition.collision_model.index. */

#include <stdint.h>
#include "headers/collision_model_instance.h"
#include "headers/_object_definition.h"
#include "headers/object_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"

typedef struct real_matrix4x3 real_matrix4x3;
extern real_matrix4x3 *object_get_node_matrices(int object_index);

uint8_t collision_model_instance_new(collision_model_instance *instance, int object_index)
{
    object_datum *object = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);
    _object_definition *object_def = TAG_GET(_object_definition, object->definition_index);

    if ( object_def->collision_model.index == -1 )
        return 0;

    instance->object_index = object_index;
    instance->model = TAG_GET(const collision_model, object_def->collision_model.index);
    instance->region_permutation_indices = object->object.region_permutations;   /* object+384 = _object_datum.region_permutations */
    instance->matrices = object_get_node_matrices(object_index);
    return 1;
}

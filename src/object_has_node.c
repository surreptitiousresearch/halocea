/* object_has_node @0x836EC0A8 — test whether an object has a given model node.
 *
 * Resolves the object's definition tag, then its model tag reference (_object_definition
 * model.index @ +0x34; DB-verified — an earlier comment mislabelled this as the
 * animation graph). If the object has no model, only node 0 is considered valid.
 * Otherwise the node index is valid when it is in [0, model->nodes.count). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/_object_definition.h"
#include "headers/model.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"

int object_has_node(int object_index, int16_t node_index)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    _object_definition *definition = TAG_GET(_object_definition, object->definition_index);
    int model_index = definition->model.index; /* was definition[13] (+0x34) */
    if ( model_index == -1 )
        return node_index == 0;

    if ( node_index < 0 )
        return 0;

    model *model_definition = TAG_GET(model, model_index);
    return node_index < model_definition->nodes.count; /* was dword +0xB8 */
}

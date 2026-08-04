/* object_type_postprocess_node_matrices @ 0x837020A8 — fan the post-pose node-matrix fixup out to
 * every part definition of the object's type (super-type plus sub-types), stopping at the null
 * terminator. Each part may override datum_postprocess_node_matrices (e.g. to apply hard-coded IK or
 * recoil to the freshly-built node matrices). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_type_definition.h"
#include "headers/real_matrix4x3.h"
#include "headers/blam_data_globals.h"


void object_type_postprocess_node_matrices(int object_index, real_matrix4x3 *node_matrices)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    object_type_definition *type_definition = object_type_definitions[object->object.type];

    for ( int i = 0; type_definition->part_definitions[i]; ++i )
    {
        void (*datum_postprocess_node_matrices)(int, real_matrix4x3 *) =
            type_definition->part_definitions[i]->datum_postprocess_node_matrices;
        if ( datum_postprocess_node_matrices )
            datum_postprocess_node_matrices(object_index, node_matrices);
    }
}

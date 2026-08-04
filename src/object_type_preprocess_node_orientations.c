/* object_type_preprocess_node_orientations @ 0x83701FF8 — fan the pre-pose node-orientation fixup out
 * to every part definition of the object's type (super-type plus sub-types), stopping at the null
 * terminator. Each part may override datum_preprocess_node_orientations to adjust the base/animation
 * orientations before the node matrices are built. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_type_definition.h"
#include "headers/real_orientation.h"
#include "headers/blam_data_globals.h"


void object_type_preprocess_node_orientations(int object_index, real_orientation *node_orientations)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    object_type_definition *type_definition = object_type_definitions[object->object.type];

    for ( int i = 0; type_definition->part_definitions[i]; ++i )
    {
        void (*datum_preprocess_node_orientations)(int, real_orientation *) =
            type_definition->part_definitions[i]->datum_preprocess_node_orientations;
        if ( datum_preprocess_node_orientations )
            datum_preprocess_node_orientations(object_index, node_orientations);
    }
}

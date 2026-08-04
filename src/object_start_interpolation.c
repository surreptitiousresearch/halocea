/* object_start_interpolation @0x836EC278 — begin interpolating an object's animated node matrices toward
 * their new pose over frame_count frames: snapshot the current node orientations (copying the
 * original_node_orientations block from the node_orientations block, one 32-byte record per node) and arm
 * the interpolation counter if the requested span is at least as long as any already in progress.
 * Deviation: the old transcription labeled the model tag lookup "animation graph"; the DB shows the
 * definition's model reference ([13] = model.index) and model->nodes.count ([46] @ 0xB8). */

#include <stdint.h>
#include <string.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/model.h"
#include "headers/blam_data_globals.h"


void object_start_interpolation(int object_index, int16_t frame_count)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    _object_definition *definition =
        TAG_GET(_object_definition, object->definition_index);
    model *model_definition = TAG_GET(model, definition->model.index);
    int16_t node_count = model_definition->nodes.count;

    memcpy((char *)object + object->object.original_node_orientations.offset,
           (const char *)object + object->object.node_orientations.offset,
           32 * node_count);

    if ( frame_count >= object->object.animation.interpolation_frame_count
                        - object->object.animation.interpolation_frame_index )
    {
        object->object.animation.interpolation_frame_count = frame_count;
        object->object.animation.interpolation_frame_index = 0;
    }
}

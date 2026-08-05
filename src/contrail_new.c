/* contrail_new @0x8373FBB8 — attach a contrail to an object marker. Allocates a contrail datum,
 * records definition/object/attachment, reads the density-function index from the object definition's
 * attachment record (definition dword 81 = attachments block, 72-byte records, word @+48), primes the
 * first frame and the four point lists, then — if the density function evaluates non-zero — marks the
 * contrail active and seeds an initial point. Returns the contrail index, or -1. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/contrail_datum.h"
#include "headers/_object_definition.h"
#include "headers/object_attachment_definition.h"
#include "headers/contrail_active_flags.h"
#include "headers/blam_data_globals.h"


extern int datum_new(data_array *data);
extern void *datum_get(data_array *array, int index);
extern void contrail_next_frame(contrail_datum *contrail);
extern uint8_t object_get_function_value(int object_index, int16_t function_index, float *value_reference);
extern void contrail_add_points(uint16_t contrail_index, int16_t contrail_point_count, uint8_t force);

int contrail_new(int definition_index, int object_index, int16_t attachment_index)
{
    if ( definition_index == -1 )
        return -1;

    int contrail_index = datum_new(contrail_data);
    if ( contrail_index == -1 )
        return contrail_index;

    contrail_datum *contrail = datum_get(contrail_data, contrail_index);
    object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    contrail->flags = 0;
    contrail->definition_index = definition_index;
    contrail->attachment_index = attachment_index;
    contrail->object_index = object_index;

    _object_definition *object_def = TAG_GET(_object_definition, object_data->definition_index);
    /* the attachment's primary scale function drives contrail density */
    object_attachment_definition *attachment =
        &((object_attachment_definition *)object_def->attachments.address)[attachment_index];
    int16_t density_function = attachment->primary_scale_function_reference;
    contrail->sequence_index = -1;
    contrail->density_function_index = density_function - 1;
    contrail_next_frame(contrail);

    contrail->texture_offset_u = 0.0;
    contrail->texture_offset_v = 0.0;
    for ( int i = 0; i < 4; i = (int16_t)(i + 1) )
    {
        contrail->contrail_point_counts[i] = 0;
        contrail->first_contrail_point_indices[i] = -1;
    }

    if ( object_get_function_value(contrail->object_index, contrail->density_function_index, &contrail->density) != 0 )
    {
        contrail->flags |= (1u << _contrail_active_bit);
        contrail_add_points(contrail_index, 1, 1u);
    }
    return contrail_index;
}

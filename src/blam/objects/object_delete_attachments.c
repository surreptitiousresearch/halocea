/* object_delete_attachments @ 0x836F1450 — Blam engine.
 * Deletes the widgets and attachments of an object. Fetches the object's type
 * definition first (via object.type at +180) — matching the original's call
 * order — then deletes widgets and attachments. Uses the object-header datum
 * index idiom. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_type_definition.h"

extern object_type_definition *object_type_definition_get(int16_t object_type);
extern void widgets_delete(int object_index);
extern void attachments_delete(int object_index);

void object_delete_attachments(int object_index)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    object_type_definition_get(object->object.type); /* was *(short *)(object + 180) */
    widgets_delete(object_index);
    attachments_delete(object_index);
}

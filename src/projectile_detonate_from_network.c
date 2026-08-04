/* projectile_detonate_from_network @0x8375ACD8 — (client) apply a "projectile detonate" message-delta:
 * decode the translated object index + detonation position, resolve to a local object, drop its
 * translated-index registration (unless persistent-object flag +2 bit 0x8 is set), move it to the decoded
 * position, detonate it, bump its own +560 field to at least 2, and delete the object.
 *
 * DEVIATION: the decompiler splits the decode destination into two separate locals (a bare int and a
 * `real_point3d[5]` array); they are one contiguous by-ref decode buffer (translated_index + position),
 * reproduced here as a single struct — same pattern as other *_from_network decode call sites in this
 * codebase. It also re-reads the object pointer from the header table (header_entry+8) instead of reusing
 * the pointer already fetched via object_try_and_get_and_verify_type — same header slot, no intervening
 * write, so it is the same value; reproduced here by reusing the pointer directly. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/field_properties_definition.h"
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/real_point3d.h"
#include "headers/object_header_datum.h"
#include "headers/projectile_datum.h"
#include "headers/object_header_flags.h"
#include "headers/projectile_action.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index);
extern int field_translated_index_unregister_local_index(const _field_properties_definition *field_properties_definition, int local_index);

extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void gearbox_object_translate(int object_index, const real_point3d *new_position);
extern void projectile_detonate(int projectile_index, uint8_t first_collision, float time_left);
extern void object_delete(int object_index);

typedef struct _projectile_detonate_message
{
    int          translated_object_index;
    real_point3d position;
} _projectile_detonate_message;

void projectile_detonate_from_network(message_delta_processor_header *header, network_game_client *client)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    _projectile_detonate_message message;
    if ( !message_delta_processor_decode_stateless(&message, header) )
        return;

    int object_index = field_translated_index_get_local_index(&field_properties_object_index_definition,
        message.translated_object_index);
    if ( object_index == -1 )
        return;

    object_header_datum *header_entry = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);

    if ( (header_entry->flags & (1u << _object_header_being_deleted_bit)) == 0 )
        field_translated_index_unregister_local_index(&field_properties_object_index_definition, object_index);

    projectile_datum *object = object_try_and_get_and_verify_type(object_index, object_mask_projectile);
    if ( !object )
        return;

    object->object.datum_role = _networked_datum_autonomous;
    gearbox_object_translate(object_index, &message.position);
    projectile_detonate(object_index, 0, 0.0f);

    if ( object->projectile.action < _projectile_action_disappear )
        object->projectile.action = _projectile_action_disappear;

    object_delete(object_index);
}

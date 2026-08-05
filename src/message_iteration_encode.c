/* message_iteration_encode @0x837A1310 — encode one iteration of a message-delta message: first every header
 * field (always written, into the iteration data stream), then every body field (via
 * encode_iterated_body_field). Per-field bit costs are accumulated and reported to the metrics layer.
 *
 * The returned status differs by mode: in stateless mode it is the AND of all field encodes (everything must
 * succeed); in incremental mode it is the OR (the iteration is "dirty" if any field changed). The running
 * accumulator `status_accumulator` is seeded to 1 for stateless and 0 for incremental, matching that AND/OR
 * semantics. If any header field fails in stateless mode the body is skipped. */

#include <stdint.h>
#include "headers/iterated_message.h"
#include "headers/message_definition_field_reference_set.h"
#include "headers/message_delta_metrics.h"
#include <string.h>
#include "headers/blam_data_globals.h"

extern _message_definition *message_delta_global_message_list[47];

extern void metrics_record_header_field_encode(const _message_definition *definition, message_delta_processor_mode mode, int field_index, int bits);
extern uint8_t encode_iterated_body_field(iterated_message *message, int field_index, char *data, char *baseline);

int message_iteration_encode(iterated_message *message, char *header, const void *data, const void *baseline)
{
    int header_success = 1;
    _message_definition *definition = message_delta_global_message_list[message->definition_type];
    _message_definition_field_reference_set *header_fields = definition->header_fields;

    if ( header_fields->count > 0 )
    {
        int field_index = 0;
        int count = header_fields->count;
        bitstream_t *data_stream = &message->iteration_data_stream;
        int reference_index = 0;
        do
        {
            _message_definition *def = message_delta_global_message_list[message->definition_type];
            int bits = def->header_fields->field_references[reference_index].properties->encode(
                def->header_fields->field_references[reference_index].properties,
                0,
                &header[def->header_fields->field_references[reference_index].offset],
                data_stream);

            char wrote;
            if ( bits <= 0 )
            {
                wrote = 0;
            }
            else
            {
                wrote = 1;
                message->iteration_header_bits_written += bits;
            }

            metrics_record_header_field_encode(def, message->mode, field_index, bits);
            header_success = (wrote && (unsigned char)header_success) ? 1 : 0;
            ++field_index;
            ++reference_index;
        }
        while ( field_index < count );
    }

    if ( !(unsigned char)header_success )
        return header_success;

    int body_count = definition->body_fields.count;
    memset(last_iteration_field_map, 0, sizeof(last_iteration_field_map));

    int field_index = 0;
    /* seed: stateless -> 1 (AND), incremental -> 0 (OR) */
    int status_accumulator =
        message->mode - 1 - (message->mode - 2 + (message->mode == _message_delta_mode_incremental));
    for ( ; field_index < body_count; )
    {
        /* const-boundary: encode_iterated_body_field feeds data/baseline into the field's
           non-const `void *` encode funcptr (shared encode/decode); drop the read-only view's const */
        uint8_t encoded = encode_iterated_body_field(message, field_index, (char *)data, (char *)baseline);
        uint8_t next;
        if ( message->mode == _message_delta_mode_incremental )
            next = ((unsigned char)status_accumulator || encoded) ? 1 : 0;
        else
            next = ((unsigned char)status_accumulator && encoded) ? 1 : 0;
        status_accumulator = next;
        ++field_index;
    }
    return status_accumulator;
}

/* message_delta_processor_parameters_reload @0x837A1AD8 — re-runs the per-field-type parameter verifier for
 * every field of every registered message-delta message (both header and body field sets). Walks the global
 * message list to its terminating sentinel; each field's verifier is looked up by the field-type id at the
 * front of its properties block.
 *
 * DEVIATION: the field-type id lives at offset 0 of _field_properties_definition (the existing header types
 * it as a _field_type pointer); it is read here as the raw int index into the field-type table, matching the
 * disassembly.
 * DEVIATION: attested void — the decompiler's `result` chain is the r3 left over from the last verifier
 * `bctrl` (837A1BB4) surviving the epilogue; the sole caller (sv_parameters_reload @0x83765EE8) discards
 * it, so the verifier results are dropped, not returned. */

#include "headers/message_definition.h"
#include "headers/field_type_definition.h"
#include "headers/blam_data_globals.h"

extern _message_definition **message_delta_global_message_list;
extern int _real_bd036d41;   /* symbol immediately after the message list — loop terminator */

static void verify_field_set(const _message_definition_field_reference_set *set)
{
    if (set->count <= 0)
        return;

    const _field_reference_definition *field = set->field_references;
    for (int i = 0; i < set->count; ++i, ++field)
    {
        if (!field->offset && !field->baseline_offset && !field->properties)
            break;
        int field_type = *(const int *)field->properties;
        message_delta_global_field_type_list[field_type].field_parameters_verifier(field->properties);
    }
}

void message_delta_processor_parameters_reload(void)
{
    for (_message_definition **entry = message_delta_global_message_list;
         (int)entry < (int)&_real_bd036d41;
         ++entry)
    {
        _message_definition *message = *entry;
        verify_field_set(message->header_fields);
        verify_field_set(&message->body_fields);
    }
}

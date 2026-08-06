/* message_delta_processor_dispose @0x837A2878 — global teardown of the message-delta system: disposes and
 * clears every message definition's field reference sets (see dispose_messages.c), clears every field
 * type's initialized flag, and shuts down the protocol-changeover parameter config system if it was active.
 *
 * DEVIATION: disasm_range(0x837A2878,0x837A28FC) confirms both loop bounds are compile-time-constant
 * addresses that happen to coincide with unrelated neighboring data symbols (`__real_bd036d41` for the
 * 47-entry message list — see dispose_messages.c's own note — and `gITEM_PLACEMENT_BITS_Z` for the 28-entry
 * field type list); reproduced as plain counted loops over the DB-declared array sizes rather than the
 * coincidental symbol arithmetic. */

#include "headers/message_definition.h"
#include "headers/field_type_definition.h"
#include "headers/blam_data_globals.h"

typedef struct _message_definition_field_reference_set _message_definition_field_reference_set;
extern void metrics_dispose(void);
extern void message_definition_field_reference_set_dispose(_message_definition_field_reference_set *const field_reference_set);
extern void message_delta_parameters_protocol_dump_to_config_file(void);
extern void message_delta_parameters_protocol_shutdown(void);

void message_delta_processor_dispose(void)
{
    metrics_dispose();

    for ( int i = 0; i < 47; ++i )
    {
        _message_definition *message_definition = message_delta_global_message_list[i];

        message_definition_field_reference_set_dispose(message_definition->header_fields);
        message_definition_field_reference_set_dispose(&message_definition->body_fields);
        message_definition->initialized = 0;
    }

    for ( int i = 0; i < 28; ++i )
        message_delta_global_field_type_list[i].initialized = 0;

    if ( g_protocol_changeover_enabled == 1 )
    {int config_file;
    message_delta_parameters_protocol_dump_to_config_file();
        message_delta_parameters_protocol_shutdown();
    }
}

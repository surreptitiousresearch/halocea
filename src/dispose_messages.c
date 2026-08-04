/* dispose_messages @0x837A2518 — dispose every message definition's field reference sets and clear its
 * initialized flag. Loop bound derived from disasm: message_delta_global_message_list spans 47 entries
 * (0x82129A10..0x82129ACC, 188 bytes / 4 = 47 pointers). */

#include "headers/message_definition.h"

typedef struct _message_definition_field_reference_set _message_definition_field_reference_set;
extern _message_definition *message_delta_global_message_list[47];
extern void message_definition_field_reference_set_dispose(_message_definition_field_reference_set *const field_reference_set);

void dispose_messages(void)
{
    for (int i = 0; i < 47; ++i)
    {
        _message_definition *message_definition = message_delta_global_message_list[i];

        message_definition_field_reference_set_dispose(message_definition->header_fields);
        message_definition_field_reference_set_dispose(&message_definition->body_fields);
        message_definition->initialized = 0;
    }
}

/* initialize_messages @0x837A2B28 — one-time setup of every message-delta message definition. Loop bound
 * derived from disasm: message_delta_global_message_list spans 47 entries (per dispose_messages.c). Per-
 * entry logic is message_definition_initialize.c's body (bit accounting, field reference set init).
 *
 * DEVIATION: returns void — r3 at the blr is only the last message's header-fields maximum-size calculation
 * leftover (no path writes r3 deliberately) and the sole caller message_delta_processor_initialize @837A2C50
 * ignores it. */

#include "headers/message_definition.h"

extern _message_definition *message_delta_global_message_list[47];
extern void message_definition_initialize(_message_definition *const message_definition);

void initialize_messages(void)
{
    for ( int i = 0; i < 47; ++i )
        message_definition_initialize(message_delta_global_message_list[i]);
}

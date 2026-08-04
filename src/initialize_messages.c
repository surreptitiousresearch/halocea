/* initialize_messages @0x837A2B28 — one-time setup of every message-delta message definition. Loop bound
 * derived from disasm: message_delta_global_message_list spans 47 entries (per dispose_messages.c). Per-
 * entry logic is message_definition_initialize.c's body (bit accounting, field reference set init).
 *
 * The compiled function's return value is just whatever the LAST message's header-fields maximum-size
 * calculation happened to return (a leftover register, not a meaningful result — no caller was found to
 * use it); message_definition_initialize.c doesn't expose that intermediate, so this returns 1. */

#include "headers/message_definition.h"

extern _message_definition *message_delta_global_message_list[47];
extern void message_definition_initialize(_message_definition *const message_definition);

int initialize_messages(void)
{
    for ( int i = 0; i < 47; ++i )
        message_definition_initialize(message_delta_global_message_list[i]);
    return 1;
}

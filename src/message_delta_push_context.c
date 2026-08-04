/* message_delta_push_context @0x837A0838 — tail-call forwarder (`b metrics_push_context`; confirmed
 * via disasm, not a bare blr). metrics_push_context itself is not yet decompiled — declared extern
 * with its own DB prototype and called through with the same argument. */

#include <stdint.h>

extern uint8_t metrics_push_context(const char *const context_name);

uint8_t message_delta_push_context(const char *const context_name)
{
    return metrics_push_context(context_name);
}

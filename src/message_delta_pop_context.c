/* message_delta_pop_context @0x837A0840 — pop the metrics context stack and verify the popped context's name
 * matches the expected one. */

#include <stdint.h>
#include <string.h>

extern const char * metrics_pop_context(void);

uint8_t message_delta_pop_context(const char *context_name)
{
    const char *popped_context_name = metrics_pop_context();

    if ( !popped_context_name )
        return 0;

    return strcmp(popped_context_name, context_name) == 0;
}

/* message_delta_metrics_dump_evaluate @0x8372D278 — HaloScript builtin wrapper for message_delta_metrics_dump; one string. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void message_delta_metrics_dump(const char *filename);

void message_delta_metrics_dump_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    const char **arguments = (const char **)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        message_delta_metrics_dump(*arguments);
        hs_return(thread_index, 0);
    }
}

/* net_graph_change_display_evaluate @0x8372D160 — HaloScript builtin wrapper for net_graph_change_display;
 * (long, long) returning bool. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
/* extern corrected to match def (net_graph_change_display.c): const char* args, returns int. */
extern uint8_t net_graph_change_display(const char *type_string, const char *direction_string);

void net_graph_change_display_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        *(unsigned char *)&result = net_graph_change_display((const char *)arguments[0], (const char *)arguments[1]);
        hs_return(thread_index, result);
    }
}

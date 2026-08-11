/* team_change_to_server_evaluate @0x8372E228 — HaloScript builtin wrapper for team_change_to_server; (short). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void team_change_to_server(uint8_t new_team_choice);

void team_change_to_server_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        team_change_to_server(*(int16_t *)arguments);
        hs_return(thread_index, 0);
    }
}

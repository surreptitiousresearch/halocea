/* network_game_client_attempt_connection_ascii_password_evaluate @0x8372E400 — HaloScript builtin wrapper.
 * The builtin evaluates its arguments and, on success, returns; the disassembly shows no further engine call
 * (the connection request is consumed entirely within argument evaluation). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);

void network_game_client_attempt_connection_ascii_password_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    if ( hs_macro_function_evaluate(function_index, thread_index, initialize) )
        hs_return(thread_index, 0);
}

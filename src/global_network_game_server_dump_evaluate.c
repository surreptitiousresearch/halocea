/* global_network_game_server_dump_evaluate @0x8372D0B8 — HaloScript builtin evaluator. Takes no arguments; invokes the engine routine
 * and returns void.
 *
 * DEVIATION: the decompiler shows the engine call with three leftover-register arguments; global_network_game_server_dump actually
 * takes none, so it is called with its real signature. */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern void global_network_game_server_dump(void);

void global_network_game_server_dump_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    global_network_game_server_dump();
    hs_return(thread_index, 0);
}

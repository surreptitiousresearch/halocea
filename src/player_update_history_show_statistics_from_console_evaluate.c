/* player_update_history_show_statistics_from_console_evaluate @0x8372D208 — HaloScript builtin wrapper.
 *
 * DEVIATION (2026-07-31): player_update_history_show_statistics_from_console is a no-argument builtin
 * (disasm: no arg regs read before written); the decompiler renders it as taking the evaluator's
 * (function_index, thread_index, initialize) triple, but it is declared and called zero-arg here. Its
 * network_game_client* return is ignored by the wrapper. */

#include <stdint.h>

typedef struct network_game_client network_game_client;
extern void hs_return(uint16_t thread_index, int value);
extern void player_update_history_show_statistics_from_console(void);

void player_update_history_show_statistics_from_console_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    (void)function_index;
    (void)initialize;
    player_update_history_show_statistics_from_console();
    hs_return(thread_index, 0);
}

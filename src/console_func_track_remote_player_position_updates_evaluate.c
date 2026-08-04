/* console_func_track_remote_player_position_updates_evaluate @0x8372E2E8 — HaloScript builtin evaluator.
 * Argument block: [0] = remote player name (string pointer). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void console_func_track_remote_player_position_updates(char *remote_player_name);

void console_func_track_remote_player_position_updates_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    char **arguments = (char **)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        console_func_track_remote_player_position_updates(*arguments);
        hs_return(thread_index, 0);
    }
}

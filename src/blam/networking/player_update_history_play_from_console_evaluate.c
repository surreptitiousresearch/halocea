/* player_update_history_play_from_console_evaluate @0x8372D1B8 — HaloScript builtin wrapper for
 * player_update_history_play_from_console. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void player_update_history_play_from_console(int last_completed_update_id, uint8_t destructive);

void player_update_history_play_from_console_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        player_update_history_play_from_console(arguments[0], ((unsigned char *)arguments)[4]);
        hs_return(thread_index, 0);
    }
}

/* hs_teleport_players_not_in_trigger_volume_evaluate @0x83727210 — HaloScript builtin evaluator. Argument
 * [0] = trigger volume index (short), [4] = scenario flag index (short). No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void hs_teleport_players_not_in_trigger_volume(int16_t trigger_volume_index, int16_t flag_index);
void hs_teleport_players_not_in_trigger_volume_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int16_t *arguments = (int16_t *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { hs_teleport_players_not_in_trigger_volume(arguments[0], arguments[2]); hs_return(thread_index, 0); }
}

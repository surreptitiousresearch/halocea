/* player_control_action_test_move_relative_all_directions_evaluate @0x8372BBB0 — HaloScript builtin wrapper (no arguments); packs a boolean result. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern uint8_t player_control_action_test_move_relative_all_directions(void);

void player_control_action_test_move_relative_all_directions_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    *(unsigned char *)&result = player_control_action_test_move_relative_all_directions();
    hs_return(thread_index, result);
}

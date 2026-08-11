/* player_control_action_test_look_relative_right_evaluate @0x8372BB20 — HaloScript builtin wrapper (no arguments); packs a boolean result. */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern uint8_t player_control_action_test_look_relative_right(void);

void player_control_action_test_look_relative_right_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    *(unsigned char *)&result = player_control_action_test_look_relative_right();
    hs_return(thread_index, result);
}

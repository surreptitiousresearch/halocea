/* player_control_action_test_accept_evaluate @0x8372B9B8 — HaloScript builtin wrapper (no arguments); packs a boolean result. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern uint8_t player_control_action_test_accept(void);

void player_control_action_test_accept_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    *(unsigned char *)&result = player_control_action_test_accept();
    hs_return(thread_index, result);
}

/* player_control_action_test_primary_trigger_evaluate @0x8372B898 — HaloScript builtin wrapper (no arguments); packs a boolean result. */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern uint8_t player_control_action_test_primary_trigger(void);

void player_control_action_test_primary_trigger_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    *(unsigned char *)&result = player_control_action_test_primary_trigger();
    hs_return(thread_index, result);
}

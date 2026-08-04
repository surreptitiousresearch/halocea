/* player0_joystick_set_is_normal_evaluate @0x8372DE60 — HaloScript builtin wrapper (no arguments); packs a boolean result. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern uint8_t player0_joystick_set_is_normal(void);

void player0_joystick_set_is_normal_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    *((unsigned char *)&result + 3) = player0_joystick_set_is_normal();
    hs_return(thread_index, result);
}

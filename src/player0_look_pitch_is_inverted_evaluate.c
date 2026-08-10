/* player0_look_pitch_is_inverted_evaluate @0x8372DE18 — HaloScript builtin wrapper (no arguments); packs a boolean result. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern uint8_t player0_look_pitch_is_inverted(void);

void player0_look_pitch_is_inverted_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    *(unsigned char *)&result = player0_look_pitch_is_inverted();
    hs_return(thread_index, result);
}

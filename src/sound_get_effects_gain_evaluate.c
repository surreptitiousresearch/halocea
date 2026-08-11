/* sound_get_effects_gain_evaluate @0x8372CC28 — HaloScript builtin wrapper (no arguments); packs a real result. */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern float sound_get_effects_gain(void);

void sound_get_effects_gain_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result;
    *(float *)&result = sound_get_effects_gain();
    hs_return(thread_index, result);
}

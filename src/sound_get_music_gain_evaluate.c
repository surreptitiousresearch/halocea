/* sound_get_music_gain_evaluate @0x8372CBA0 — HaloScript builtin wrapper (no arguments); packs a real result. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern float sound_get_music_gain(void);

void sound_get_music_gain_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result;
    *(float *)&result = sound_get_music_gain();
    hs_return(thread_index, result);
}

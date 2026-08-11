/* game_safe_to_speak_evaluate @0x8372C388 — HaloScript builtin evaluator. Takes no arguments; returns a boolean packed into
 * the high byte of the result word. */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern uint8_t game_safe_to_speak(void);

void game_safe_to_speak_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    *(unsigned char *)&result = game_safe_to_speak();
    hs_return(thread_index, result);
}

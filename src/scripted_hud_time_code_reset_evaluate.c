/* scripted_hud_time_code_reset_evaluate @0x8372D970 — HaloScript builtin evaluator. Stub: returns void (HUD time-code display is
 * compiled out of this build). */
#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
void scripted_hud_time_code_reset_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_return(thread_index, 0);
}

/* scripted_camera_time_evaluate @0x8372B578 — HaloScript builtin evaluator. No arguments; returns the
 * scripted camera's remaining ticks as a short packed into the high word of the result (low word zero). */
#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern uint16_t scripted_camera_time(void);
void scripted_camera_time_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    *(int16_t *)&result = scripted_camera_time();
    hs_return(thread_index, result);
}

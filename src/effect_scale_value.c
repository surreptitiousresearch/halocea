/* effect_scale_value @0x83738530 */
#include <stdint.h>

extern float transition_function_evaluate(int16_t function_type, float value);

float effect_scale_value(int16_t transition, float scale, float dt, float duration)
{
    return transition_function_evaluate(transition, 1.0f - dt / duration) * scale;
}

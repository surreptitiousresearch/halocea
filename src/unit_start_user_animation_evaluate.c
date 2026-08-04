/* unit_start_user_animation_evaluate @0x83728640 — HaloScript builtin wrapper; packs a boolean result. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern uint8_t unit_start_user_animation(int unit_index, int animation_graph_index, const char *animation_name, uint8_t interpolate);

void unit_start_user_animation_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        *((unsigned char *)&result + 3) = unit_start_user_animation(
            /* HS packed arg block: byte +12 is the boolean interpolate arg (BE high byte of arguments[3]) */
            arguments[0], arguments[1], (const char *)arguments[2], ((unsigned char *)arguments)[12]);
        hs_return(thread_index, result);
    }
}

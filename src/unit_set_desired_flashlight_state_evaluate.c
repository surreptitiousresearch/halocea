/* unit_set_desired_flashlight_state_evaluate @0x83729140 — HaloScript builtin wrapper for unit_set_desired_flashlight_state. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void unit_set_desired_flashlight_state(int unit_index, uint8_t state);

void unit_set_desired_flashlight_state_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* HS packed arg block: byte +4 is the boolean second argument (BE high byte of arguments[1]) */
        unit_set_desired_flashlight_state(arguments[0], ((unsigned char *)arguments)[4]);
        hs_return(thread_index, 0);
    }
}

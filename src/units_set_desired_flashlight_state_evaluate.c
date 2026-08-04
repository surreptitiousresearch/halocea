/* units_set_desired_flashlight_state_evaluate @0x837290F0 — HaloScript builtin wrapper for units_set_desired_flashlight_state. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void units_set_desired_flashlight_state(int object_list_index, uint8_t state);

void units_set_desired_flashlight_state_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* HS argument block: packed wire, no DB struct — unit_index at [0], flag byte at +4 */
        units_set_desired_flashlight_state(arguments[0], ((unsigned char *)arguments)[4]);
        hs_return(thread_index, 0);
    }
}

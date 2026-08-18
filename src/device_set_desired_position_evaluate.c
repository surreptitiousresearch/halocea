/* device_set_desired_position_evaluate @0x837292D8 — HaloScript builtin evaluator. Argument block:
 * [0] = device index (long), [4] = value (float). Returns a boolean (did the value change) packed into the
 * high byte of the result word. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern uint8_t device_set_desired_position(int device_index, float value);

void device_set_desired_position_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* DEVIATION: endian-portable respelling of the BE high-byte store (was an *(narrow*)&result pun; hs_inspect_boolean extracts value >> 24) 2026-08-18 */
        int result = (int)((uint8_t)(device_set_desired_position(arguments[0], ((float *)arguments)[1]))) << 24;
        hs_return(thread_index, result);
    }
}

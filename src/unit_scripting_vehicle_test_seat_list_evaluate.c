/* unit_scripting_vehicle_test_seat_list_evaluate @0x83728900 — HaloScript builtin wrapper; packs a boolean result. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern uint8_t unit_scripting_vehicle_test_seat_list(int unit_index, const char *seat_name, int object_list_index);

void unit_scripting_vehicle_test_seat_list_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        *(unsigned char *)&result = unit_scripting_vehicle_test_seat_list(
            arguments[0], (const char *)arguments[1], arguments[2]);
        hs_return(thread_index, result);
    }
}

/* unit_scripting_enter_vehicle_evaluate @0x837288B0 — HaloScript builtin wrapper. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void unit_scripting_enter_vehicle(int unit_index, int parent_unit_index, const char *seat_name);

void unit_scripting_enter_vehicle_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        unit_scripting_enter_vehicle(arguments[0], arguments[1], (const char *)arguments[2]);
        hs_return(thread_index, 0);
    }
}

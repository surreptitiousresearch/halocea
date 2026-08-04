/* vehicle_scripting_unload_evaluate @0x83728C18 — HaloScript builtin wrapper for vehicle_scripting_unload;
 * (object, string) returning a short. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern int16_t vehicle_scripting_unload(int unit_index, const char *seat_substring_name);

void vehicle_scripting_unload_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result;
    *((__int16 *)&result) = 0;
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        *((__int16 *)&result + 1) = vehicle_scripting_unload(arguments[0], (const char *)arguments[1]);
        hs_return(thread_index, result);
    }
}

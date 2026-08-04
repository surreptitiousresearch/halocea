/* vehicle_scripting_load_magic_evaluate @0x83728BB8 — HaloScript builtin wrapper for vehicle_scripting_load_magic;
 * (object, string, object) returning a short. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern __int16 vehicle_scripting_load_magic(int unit_index, const char *seat_substring_name, int object_list_index);

void vehicle_scripting_load_magic_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result;
    *((__int16 *)&result) = 0;
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        *((__int16 *)&result + 1) = vehicle_scripting_load_magic(arguments[0], (const char *)arguments[1], arguments[2]);
        hs_return(thread_index, result);
    }
}

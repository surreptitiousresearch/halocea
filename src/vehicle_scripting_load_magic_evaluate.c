/* vehicle_scripting_load_magic_evaluate @0x83728BB8 — HaloScript builtin wrapper for vehicle_scripting_load_magic;
 * (object, string, object) returning a short. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern int16_t vehicle_scripting_load_magic(int unit_index, const char *seat_substring_name, int object_list_index);

void vehicle_scripting_load_magic_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;   /* DEVIATION: `stw r11, 0x50(r1)` @0x83728BD0 zeroes the WHOLE word — the halfword */
    /* form left slot bytes 2-3 uninitialised; `sth 0x50(r1)` @0x83728BF0 puts the short at slot halfword 0. */
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        *(int16_t *)&result = vehicle_scripting_load_magic(arguments[0], (const char *)arguments[1], arguments[2]);
        hs_return(thread_index, result);
    }
}

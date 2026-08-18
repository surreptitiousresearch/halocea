/* scenario_trigger_volume_test_object_evaluate @0x83727260 — HaloScript builtin wrapper; (short, object) returning bool. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern uint8_t scenario_trigger_volume_test_object(int16_t trigger_volume_index, int object_index);

void scenario_trigger_volume_test_object_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* DEVIATION: endian-portable respelling of the BE high-byte store (was an *(narrow*)&result pun; hs_inspect_boolean extracts value >> 24) 2026-08-18 */
        int result = (int)((uint8_t)(scenario_trigger_volume_test_object(*(int16_t *)arguments, arguments[1]))) << 24;
        hs_return(thread_index, result);
    }
}

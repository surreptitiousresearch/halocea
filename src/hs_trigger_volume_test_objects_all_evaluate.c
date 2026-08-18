/* hs_trigger_volume_test_objects_all_evaluate @0x83727310 — HaloScript builtin evaluator. Argument [0] = trigger volume index (short),
 * [4] = object list (long). Returns a boolean packed into the high byte of the result word. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern uint8_t hs_trigger_volume_test_objects_all(int16_t trigger_volume_index, int object_list_index);
void hs_trigger_volume_test_objects_all_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* DEVIATION: endian-portable respelling of the BE high-byte store (was an *(narrow*)&result pun; hs_inspect_boolean extracts value >> 24) 2026-08-18 */
        int result = (int)((uint8_t)(hs_trigger_volume_test_objects_all(*(int16_t *)arguments, arguments[1]))) << 24;
        hs_return(thread_index, result);
    }
}

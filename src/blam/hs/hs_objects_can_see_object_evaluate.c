/* hs_objects_can_see_object_evaluate @0x83727878 — HaloScript builtin evaluator. Argument [0] = object list
 * (long), [4] = target object (long), [8] = cone half-angle degrees (float). Returns a boolean packed into
 * the high byte of the result word. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern uint8_t hs_objects_can_see_object(int object_list_index, int object_index, float degrees);
void hs_objects_can_see_object_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* DEVIATION: endian-portable respelling of the BE high-byte store (was an *(narrow*)&result pun; hs_inspect_boolean extracts value >> 24) 2026-08-18 */
        int result = (int)((uint8_t)(hs_objects_can_see_object(arguments[0], arguments[1], ((float *)arguments)[2]))) << 24;
        hs_return(thread_index, result);
    }
}

/* hs_objects_can_see_flag_evaluate @0x837278D8 — HaloScript builtin evaluator. Argument [0] = object list
 * (long), [4] = scenario flag index (short), [8] = cone half-angle degrees (float). Returns a boolean packed
 * into the high byte of the result word. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern uint8_t hs_objects_can_see_flag(int object_list_index, int16_t flag_index, float degrees);
void hs_objects_can_see_flag_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        *(unsigned char *)&result = hs_objects_can_see_flag(arguments[0], ((int16_t *)arguments)[2] /* +4: flag index */, ((float *)arguments)[2] /* +8: degrees (packed HS arg buffer) */);
        hs_return(thread_index, result);
    }
}

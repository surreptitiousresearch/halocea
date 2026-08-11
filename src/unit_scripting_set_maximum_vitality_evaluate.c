/* unit_scripting_set_maximum_vitality_evaluate @0x83728A58 — HaloScript builtin wrapper for unit_scripting_set_maximum_vitality. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void unit_scripting_set_maximum_vitality(int unit_index, float body_vitality, float shield_vitality);

void unit_scripting_set_maximum_vitality_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        float *argument_floats = (float *)arguments;   /* HaloScript packed argument buffer */
        unit_scripting_set_maximum_vitality(arguments[0], argument_floats[1], argument_floats[2]);
        hs_return(thread_index, 0);
    }
}

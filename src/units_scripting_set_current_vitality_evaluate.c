/* units_scripting_set_current_vitality_evaluate @0x83728B60 — HaloScript builtin wrapper for units_scripting_set_current_vitality. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void units_scripting_set_current_vitality(int object_list_index, float body_vitality, float shield_vitality);

void units_scripting_set_current_vitality_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* HS argument block: packed wire, faithfully de-flagged */
        units_scripting_set_current_vitality(arguments[0], ((float *)arguments)[1], ((float *)arguments)[2]);
        hs_return(thread_index, 0);
    }
}

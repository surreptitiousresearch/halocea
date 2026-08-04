/* unit_scripting_can_blink_evaluate @0x83728430 — HaloScript builtin wrapper for unit_scripting_can_blink. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void unit_scripting_can_blink(int unit_index, uint8_t can_blink);

void unit_scripting_can_blink_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* HS packed arg block: byte +4 is the boolean second argument (BE high byte of arguments[1]) */
        unit_scripting_can_blink(arguments[0], ((unsigned char *)arguments)[4]);
        hs_return(thread_index, 0);
    }
}

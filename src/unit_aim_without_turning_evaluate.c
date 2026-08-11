/* unit_aim_without_turning_evaluate @0x837287C0 — HaloScript builtin wrapper for unit_aim_without_turning. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void unit_aim_without_turning(int unit_index, uint8_t aim_without_turning);

void unit_aim_without_turning_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* HS packed arg block: byte +4 is the boolean second argument (BE high byte of arguments[1]) */
        unit_aim_without_turning(arguments[0], ((unsigned char *)arguments)[4]);
        hs_return(thread_index, 0);
    }
}

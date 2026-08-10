/* lights_enable_evaluate @0x837282C8 — HaloScript builtin wrapper for lights_enable; (bool) returning bool. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern uint8_t lights_enable(uint8_t enable);

void lights_enable_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        *(unsigned char *)&result = lights_enable(((unsigned char *)arguments)[0]);
        hs_return(thread_index, result);
    }
}

/* cinematic_show_letterbox_evaluate @0x8372C150 — HaloScript builtin evaluator. Argument block: [0] = boolean. No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void cinematic_show_letterbox(uint8_t show);

void cinematic_show_letterbox_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    unsigned char *arguments = (unsigned char *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        cinematic_show_letterbox(*arguments);
        hs_return(thread_index, 0);
    }
}

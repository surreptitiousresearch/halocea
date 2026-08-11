/* scripted_show_hud_help_text_evaluate @0x8372CD10 — HaloScript builtin evaluator. Argument [0] = boolean. Returns the new state
 * packed into the high byte of the result word. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern uint8_t scripted_show_hud_help_text(uint8_t flag);
void scripted_show_hud_help_text_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    unsigned char *arguments = (unsigned char *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { *(unsigned char *)&result = scripted_show_hud_help_text(*arguments); hs_return(thread_index, result); }
}

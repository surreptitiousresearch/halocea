/* ai_scripting_allow_dormant_evaluate @0x8372AEB0 — HaloScript builtin (ai_allow_dormant) evaluator. Argument
 * block: [0] = ai index (int), [4] = allow flag (boolean). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void ai_scripting_allow_dormant(int ai_index, uint8_t allow_dormant);

void ai_scripting_allow_dormant_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* RAW: HS argument block — 4-byte-per-slot union array; slot[0]=ai index, slot[1] byte-0 (BE MSB)=allow flag. */
        ai_scripting_allow_dormant(arguments[0], *(unsigned char *)((char *)arguments + 4));
        hs_return(thread_index, 0);
    }
}

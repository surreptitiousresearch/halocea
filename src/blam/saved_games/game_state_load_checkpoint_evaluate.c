/* game_state_load_checkpoint_evaluate @0x8372E388 — HaloScript builtin evaluator. Evaluates its arguments
 * and returns; the actual checkpoint reload is driven elsewhere (this builtin only gates on argument
 * evaluation in this build). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);

void game_state_load_checkpoint_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    if ( hs_macro_function_evaluate(function_index, thread_index, initialize) )
        hs_return(thread_index, 0);
}

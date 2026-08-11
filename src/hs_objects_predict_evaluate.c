/* hs_objects_predict_evaluate @0x83728128 — HaloScript builtin evaluator. Argument [0] = long. No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void hs_objects_predict(int object_list_index);
void hs_objects_predict_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { hs_objects_predict(arguments[0]); hs_return(thread_index, 0); }
}

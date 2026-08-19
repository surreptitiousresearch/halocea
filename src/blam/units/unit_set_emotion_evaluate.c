/* unit_set_emotion_evaluate @0x83728810 — HaloScript builtin wrapper for unit_set_emotion. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void unit_set_emotion(int unit_index, int16_t emotion_index);

void unit_set_emotion_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* HS packed arg block: __int16 slot +2 (byte +4) is the emotion second argument */
        unit_set_emotion(arguments[0], ((int16_t *)arguments)[2]);
        hs_return(thread_index, 0);
    }
}

/* ProfileServiceClearRecordTimes_evaluate @0x8372BC48 — HaloScript profiling builtin (ProfileService clear record times) evaluator. Retail-stubbed: the builtin body is compiled out, so the
 * evaluator simply returns 0 (it does not even fetch arguments via hs_macro_function_evaluate). */

#include <stdint.h>

extern void hs_return(int thread_index, int value);

void ProfileServiceClearRecordTimes_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_return(thread_index, 0);
}

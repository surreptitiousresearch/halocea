/* unit_custom_animation_at_frame_evaluate @0x83728700 — HaloScript builtin wrapper; packs a boolean result. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern uint8_t unit_custom_animation_at_frame(int unit_index, int animation_graph_index, const char *animation_name, uint8_t interpolate, int16_t frame_index);

void unit_custom_animation_at_frame_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* HaloScript packed argument buffer (opaque wire layout): interpolate flag byte at +12,
         * frame __int16 at +16; read at exact byte offsets to preserve big-endian semantics. */
        unsigned char *argument_bytes = (unsigned char *)arguments;
        int16_t *argument_words = (int16_t *)arguments;
        *(unsigned char *)&result = unit_custom_animation_at_frame(
            arguments[0], arguments[1], (const char *)arguments[2],
            argument_bytes[12], argument_words[8]);
        hs_return(thread_index, result);
    }
}

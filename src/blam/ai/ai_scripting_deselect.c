#include <stdint.h>
/* ai_scripting_deselect @0x8376F8C8 — no-op (the deselect script builtin has no engine effect in this
 * build; the function body is a bare return). Called directly with the evaluator's parameters, which it
 * ignores. */

void ai_scripting_deselect(int16_t function_index, int thread_index, uint8_t initialize)
{
    (void)function_index;
    (void)thread_index;
    (void)initialize;
}

/* hs_doc_evaluate @0x83730330 — HaloScript builtin evaluator. Takes no arguments; dumps all function docs and
 * returns void.
 *
 * DEVIATION: the decompiler shows the engine call with three leftover-register arguments; hs_doc takes none. */
#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern void hs_doc(void);
void hs_doc_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_doc();
    hs_return(thread_index, 0);
}

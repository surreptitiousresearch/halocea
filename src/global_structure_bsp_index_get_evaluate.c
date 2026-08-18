/* global_structure_bsp_index_get_evaluate @0x8372BDC0 — HaloScript builtin evaluator. No arguments; returns
 * the current structure BSP index as a short packed into the high word of the result (low word zero). */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern int16_t global_structure_bsp_index_get(void);

void global_structure_bsp_index_get_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    /* DEVIATION: endian-portable respelling of the BE high-halfword store (was an *(narrow*)&result pun; hs extracts value >> 16) 2026-08-18 */
    int result = (int)((uint16_t)global_structure_bsp_index_get()) << 16;
    hs_return(thread_index, result);
}

/* main_saving_map_evaluate @0x8372C4F8 — HaloScript wrapper for (main_saving_map): return the pending-save
 * boolean to the script thread.
 *
 * Deviation: the decompiler writes the bool into the high byte (HIBYTE) of the 32-bit return slot; this is
 * the engine's boolean HS-value encoding, reproduced as a << 24. */

#include <stdint.h>

extern uint8_t main_saving_map(void);
extern void hs_return(uint16_t thread_index, int value);

void main_saving_map_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = (main_saving_map() & 0xFF) << 24;
    hs_return(thread_index, result);
}

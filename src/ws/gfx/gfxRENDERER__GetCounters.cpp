/* ?GetCounters@gfxRENDERER@gfx@@QAAXAAUgfxCounters@2@@Z @0x8332DA70 */
#include "../../headers/ws/gfx/gfxRENDERER.h"

// 0x8332DA74 — snapshot the current counters. DEVIATION: the decompiler rendered this as a raw
// 9-word backwards pointer-walk memcpy (`*++p_m_NTimesTextureChangeCausedFlush = *++p_line`, 9
// times) starting one int before m_Counters and ending at its last field — a compiler-generated
// unrolled struct-copy loop, not meaningful field-by-field logic. Collapsed to the equivalent
// struct assignment (gfxCounters is 9 ints / 36 bytes, matching the loop's trip count).
void gfx::gfxRENDERER::GetCounters(gfxCounters &rCounters)
{
    rCounters = m_Counters;
}

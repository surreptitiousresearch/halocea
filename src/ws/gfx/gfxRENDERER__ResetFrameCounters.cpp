#include "../../headers/ws/gfx/gfxRENDERER.h"

// 0x8332DA54 — zero the 5 per-frame (non-cumulative) counters; m_NTextures/m_TextureMem/
// m_NPictures/m_PictureMem are left alone (they track live resource totals, not per-frame draws).
void gfx::gfxRENDERER::ResetFrameCounters()
{
    m_Counters.m_DIPs = 0;
    m_Counters.m_LongestDIPSequence = 0;
    m_Counters.m_LongestLineSequence = 0;
    m_Counters.m_LongestBitmapSequence = 0;
    m_Counters.m_NTimesTextureChangeCausedFlush = 0;
}

#pragma once
// ws-engine gfx (Scaleform GFx-hosted UI renderer, "GFxRenderer_XBOX360"-derived): a snapshot of
// draw-call statistics for one frame, exposed to the debug stat overlay.
// DB-verified layout (types_members gfx::gfxCounters): m_NTextures@0, m_TextureMem@4,
// m_NPictures@8, m_PictureMem@12, m_DIPs@16, m_LongestDIPSequence@20, m_LongestLineSequence@24,
// m_LongestBitmapSequence@28, m_NTimesTextureChangeCausedFlush@32 — all unsigned int, size 36.

namespace gfx {

typedef struct gfxCounters {
    unsigned int m_NTextures;                       // 0x00
    unsigned int m_TextureMem;                       // 0x04
    unsigned int m_NPictures;                         // 0x08
    unsigned int m_PictureMem;                        // 0x0C
    unsigned int m_DIPs;                              // 0x10 draw-indexed-primitive calls this frame
    unsigned int m_LongestDIPSequence;                // 0x14
    unsigned int m_LongestLineSequence;               // 0x18
    unsigned int m_LongestBitmapSequence;             // 0x1C
    unsigned int m_NTimesTextureChangeCausedFlush;    // 0x20
} gfxCounters;

} // namespace gfx

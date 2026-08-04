#pragma once
// ws-engine rend: builds a Morton/Z-order swizzle bit-mask triple for a WxHxD volume -- assigns
// successive power-of-two bits round-robin to the U/V/W axes (one bit per axis per round) for as
// long as that axis's dimension still exceeds the round's threshold, producing m_MaskU/V/W such
// that interleaving address bits by these masks yields swizzled/tiled memory order.
// DB-verified layout (types_members SWIZZLER) -- size 36.

typedef struct SWIZZLER
{
    unsigned int m_Width;  /* 0x00 */
    unsigned int m_Height; /* 0x04 */
    unsigned int m_Depth;  /* 0x08 */
    unsigned int m_MaskU;  /* 0x0C */
    unsigned int m_MaskV;  /* 0x10 */
    unsigned int m_MaskW;  /* 0x14 */
    unsigned int m_u;      /* 0x18 */
    unsigned int m_v;      /* 0x1C */
    unsigned int m_w;      /* 0x20 */

    // 0x82D580C4 -- reversed in SWIZZLER__Init.c.
    void Init(unsigned int width, unsigned int height, unsigned int depth);
} SWIZZLER;

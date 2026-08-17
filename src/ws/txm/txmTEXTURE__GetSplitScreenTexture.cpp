/* ?GetSplitScreenTexture@txmTEXTURE@@QAAPAV1@H@Z @0x823EF400 */
#include "../../headers/ws/txm/txmTEXTURE.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// 0x823EF400 -- ?GetSplitScreenTexture@txmTEXTURE@@QAAPAV1@H@Z
// Return one of the two split-screen sibling textures. idx must be 0 or 1.
txmTEXTURE *txmTEXTURE::GetSplitScreenTexture(unsigned int idx)
{
    if (!IGNORE_STRONG_ASSERT && idx >= 2)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("(0<=idx)&&(idx<=1)",
            "d:\\projects\\code\\common\\incl.sys\\drv\\txm.h", 354, empty_string);

    return this->pSplitScreenTex[idx];
}

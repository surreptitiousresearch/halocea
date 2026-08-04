#include "../../headers/ws/cdt/cdtSKEL.h"
#include "../../headers/ws/anim/animINST.h"
#include "../../headers/ws/obj/objOBJ.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// 0x82B0C2C0
void cdtSKEL::GetEnclBox(animINST *pInst, m3dBOX *pBox)
{
    int boneCount = bones.nElem;
    if ( boneCount >= 1 )
    {
        pBox->CalcStart();
        // The decompiler kept the loop index in two registers (r31/r25) that always move in
        // lockstep — collapsed to a single index here.
        for ( int boneIdx = 0; boneIdx < boneCount; ++boneIdx )
        {
            if ( !IGNORE_STRONG_ASSERT && (boneIdx < 0 || boneIdx >= bones.nElem) )
                STRONG_ASSERT2_HELPER::asserd(
                    "idx >= 0 && idx < nElem",
                    "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
                    567,
                    "idx", boneIdx,
                    "nElem", bones.nElem);

            cdtBONE *bone = bones.pData[boneIdx];
            objOBJ *boneObj = bone->GetObjFrom(pInst);
            // stateProc bit 0x20 marks the object as excluded from enclosing-box computation
            // (e.g. hidden/disabled) — skip it when set.
            if ( (boneObj->stateProc & 0x20) == 0 )
                bone->GetEnclBox(pInst, pBox);
        }
    }
    else
    {
        // Decompiler modeled this as a 6-dword copy through offset pointer arithmetic
        // (&pBox[-1].fur.z .. +6); that is exactly sizeof(m3dBOX) — a plain struct copy.
        *pBox = *pInst->GetBBox(1);
    }
}

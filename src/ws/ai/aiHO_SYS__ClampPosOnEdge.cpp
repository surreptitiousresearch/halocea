#include "../../headers/ws/ai/aiHO_SYS.h"
#include "../../headers/ws/ho/hoEDGE.h"        // hoEDGE::length
#include "../../headers/ws/m3d/m3d_boundary.h" // _m3dClamp (3-arg), IGNORE_STRONG_ASSERT
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern unsigned char byte_8200155A; // shared empty-string / assert-info byte @0x8200155A

// aiHO_SYS::ClampPosOnEdge @ 0x832A5B38
// ?ClampPosOnEdge@aiHO_SYS@@UAAMPAVmdlITRC_HO@@PBVhoEDGE@@M@Z
//
// Clamp `pos` onto the usable interior of `edge`: query the per-edge near/far offsets, and if the
// edge is long enough to have any interior, clamp `pos` into [near, length-far]; otherwise leave it.
float aiHO_SYS::ClampPosOnEdge(mdlITRC_HO *itrc, const hoEDGE *edge, float pos)
{
    // STRONG_ASSERT(itrc) @ line 600, STRONG_ASSERT(edge) @ line 601 (clean equivalent of the
    // decompiler's nested IGNORE_STRONG_ASSERT / short-circuit form).
    if (!IGNORE_STRONG_ASSERT && !itrc)
        STRONG_ASSERT_DUMMY::Crash(
            nullptr, "itrc",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_ho_sys.cpp", 600, byte_8200155A);
    if (!IGNORE_STRONG_ASSERT && !edge)
        STRONG_ASSERT_DUMMY::Crash(
            nullptr, "edge",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_ho_sys.cpp", 601, byte_8200155A);

    float nearOffset;   // v10
    float farOffset[3]; // v11 — GetEdgeOffsets writes a small array; only [0] is used here
    GetEdgeOffsets(itrc, edge, &nearOffset, farOffset);

    if (edge->length >= nearOffset + farOffset[0])
        return _m3dClamp(nearOffset, edge->length - farOffset[0], pos);
    return pos;
}

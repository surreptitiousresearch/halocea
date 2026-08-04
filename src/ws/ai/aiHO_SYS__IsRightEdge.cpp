#include "../../headers/ws/ai/aiHO_SYS.h"
#include "../../headers/ws/ho/hoEDGE.h"
#include "../../headers/ws/ds/dsCMP.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern unsigned char byte_8200155A; // shared assert-info byte @0x8200155A
extern int IGNORE_STRONG_ASSERT;    // ?IGNORE_STRONG_ASSERT@@3HA

// aiHO_SYS::IsRightEdge @ 0x832A61A8
// ?IsRightEdge@aiHO_SYS@@UAA_NPAVmdlITRC_HO@@PBVhoEDGE@@1AA_N@Z
//
// True when `edgeRight` is registered as a right-neighbour of `edge`. Reports whether `edge`'s
// right side is an outer edge (itrc_st2 bit 0x10). `isOuterEdge` is always written.
bool aiHO_SYS::IsRightEdge(mdlITRC_HO *itrc, const hoEDGE *edge, const hoEDGE *edgeRight, bool &isOuterEdge)
{
    if (!IGNORE_STRONG_ASSERT && !itrc)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "itrc",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_ho_sys.cpp", 643, byte_8200155A);
    if (!IGNORE_STRONG_ASSERT && !edge)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "edge",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_ho_sys.cpp", 644, byte_8200155A);
    if (!IGNORE_STRONG_ASSERT && !edgeRight)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "edgeRight",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_ho_sys.cpp", 645, byte_8200155A);

    isOuterEdge = false;
    dsCMP cmp;
    if (edge->neighborEdges2.Find(edgeRight, cmp, 0) < 0)
        return false;
    isOuterEdge = (edge->itrc_st2.state & 0x10) != 0;
    return true;
}

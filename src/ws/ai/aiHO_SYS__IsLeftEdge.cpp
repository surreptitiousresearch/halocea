#include "../../headers/ws/ai/aiHO_SYS.h"
#include "../../headers/ws/ho/hoEDGE.h"
#include "../../headers/ws/ds/dsCMP.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern unsigned char byte_8200155A; // shared assert-info byte @0x8200155A
extern int IGNORE_STRONG_ASSERT;    // ?IGNORE_STRONG_ASSERT@@3HA

// aiHO_SYS::IsLeftEdge @ 0x832A6098
// ?IsLeftEdge@aiHO_SYS@@UAA_NPAVmdlITRC_HO@@PBVhoEDGE@@1AA_N@Z
//
// True when `edgeLeft` is registered as a left-neighbour of `edge`. Also reports whether `edge`'s
// left side is an outer edge (itrc_st1 bit 0x10). `isOuterEdge` is always written.
bool aiHO_SYS::IsLeftEdge(mdlITRC_HO *itrc, const hoEDGE *edge, const hoEDGE *edgeLeft, bool &isOuterEdge)
{
    if (!IGNORE_STRONG_ASSERT && !itrc)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "itrc",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_ho_sys.cpp", 628, byte_8200155A);
    if (!IGNORE_STRONG_ASSERT && !edge)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "edge",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_ho_sys.cpp", 629, byte_8200155A);
    if (!IGNORE_STRONG_ASSERT && !edgeLeft)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "edgeLeft",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_ho_sys.cpp", 630, byte_8200155A);

    isOuterEdge = false;
    dsCMP cmp;
    if (edge->neighborEdges1.Find(edgeLeft, cmp, 0) < 0)
        return false;
    isOuterEdge = (edge->itrc_st1.state & 0x10) != 0;
    return true;
}

#include "../../headers/ws/ai/aiHO_SYS_BASE.h"
#include "../../headers/ws/ho/hoEDGE.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern unsigned char byte_8200155A; // shared assert-info byte @0x8200155A
extern int IGNORE_STRONG_ASSERT;    // ?IGNORE_STRONG_ASSERT@@3HA
extern const float M3D_INFINITY;    // m3d +inf sentinel @0x820013F4

// aiHO_SYS_BASE::BuildHOEdgePath @ 0x832A6AC0
// ?BuildHOEdgePath@aiHO_SYS_BASE@@UAA_NABUHO_EDGE_PATH_QUERY@1@AAV?$dsVECTOR@PAVhoEDGE@@$07@@@Z
//
// Build the HO-edge path for `query` into `ePath`. Trivial cases: different owner objects -> no path;
// same edge -> single-element path. Otherwise seed a local query (with is3D resolved from the itrc)
// and hand off to the recursive builder with a +inf best-length threshold.
bool aiHO_SYS_BASE::BuildHOEdgePath(const HO_EDGE_PATH_QUERY &query, dsVECTOR<hoEDGE *, 8> &ePath)
{
    if (!IGNORE_STRONG_ASSERT && (!query.eFrom || !query.eTo))
        STRONG_ASSERT_DUMMY::Crash(nullptr, "query.eFrom && query.eTo",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_ho_sys.cpp", 57, byte_8200155A);

    ePath.nElem = 0; // reset result (elements are POD pointers; the decompiler's dtor scan is a no-op)

    if (query.eFrom->owner != query.eTo->owner)
        return false;
    if (query.eFrom->id_edge == query.eTo->id_edge) {
        ePath.PushBack(query.eFrom);
        return true;
    }

    // Local copy of the query with itrc cleared and is3D resolved (matches the binary).
    HO_EDGE_PATH_QUERY local;
    local.itrc        = nullptr;
    local.is3D        = IsHO3DMode(query.itrc);
    local.eFrom       = query.eFrom;
    local.posFrom     = query.posFrom;
    local.eTo         = query.eTo;
    local.posTo       = query.posTo;
    local.havePrefDir = query.havePrefDir;
    local.prefDir     = query.prefDir;
    local.allowMove   = query.allowMove;
    local.allowFlip   = query.allowFlip;
    local.allowOuter  = query.allowOuter;

    float bestLen = M3D_INFINITY;
    return BuildHOEdgePathRec(local, ePath, 0.0f, bestLen);
}

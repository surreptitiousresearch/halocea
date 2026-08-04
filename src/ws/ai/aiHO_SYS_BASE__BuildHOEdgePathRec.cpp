#include "../../headers/ws/ai/aiHO_SYS_BASE.h"
#include "../../headers/ws/ho/hoEDGE.h"
#include "../../headers/ws/ds/dsCMP.h"
#include "../../headers/ws/m3d/m3d_boundary.h" // m3dNormalize

// aiHO_SYS_BASE::BuildHOEdgePathRec @ 0x832A6F48
// ?BuildHOEdgePathRec@aiHO_SYS_BASE@@MAA_NABUHO_EDGE_PATH_QUERY@1@AAV?$dsVECTOR@PAVhoEDGE@@$07@@MAAM@Z
//
// Recursive DFS that extends `ePath` from query.eFrom toward query.eTo across the HO edge graph,
// accumulating the traversal cost in `len` and keeping the best full-path cost found so far in
// `bestLen` (a shared threshold pruned against at every node). Returns true when it committed a
// path into `ePath` that beat `bestLen`.
//
// Deviation from the decompiler: the three candidate vectors and the path snapshots are modeled as
// RAII dsVECTOR temporaries — the decompiler renders their construction/copy/destruction as raw
// memset + Insert + dlFree with hand-carried apCL cookies. The candidate weight/length assignments,
// the neighbour-side swap under is3D, and the recursion arguments (posFrom=candLen, len+=candWeight)
// are all disasm-verified (rec call @0x832A74C4, candLen/candWeight loads @0x832A7418).
bool aiHO_SYS_BASE::BuildHOEdgePathRec(const HO_EDGE_PATH_QUERY &query, dsVECTOR<hoEDGE *, 8> &ePath,
                                       float len, float &bestLen)
{
    dsCMP cmp;
    if (ePath.Find(query.eFrom, cmp, 0) >= 0)
        return false; // edge already visited on this path

    ePath.PushBack(query.eFrom);
    hoEDGE *eFrom = query.eFrom;

    // Directional preference weight: cheaper when the edge's snap normal aligns with prefDir.
    float weight = 1.0f;
    if (query.havePrefDir) {
        m3dV snapNorm;
        eFrom->GetSnapNorm(&snapNorm);
        m3dNormalize(&snapNorm);
        float dot = query.prefDir.x * snapNorm.x
                  + query.prefDir.y * snapNorm.y
                  + query.prefDir.z * snapNorm.z;
        if (dot <= 0.5f) {
            if (dot < 0.0f)
                weight = 2.0f - dot;
        } else {
            weight = (1.0f - dot) * 0.5f + 0.44999999f;
        }
    }

    // Base case: reached the goal edge — record the improved cost.
    if (eFrom->id_edge == query.eTo->id_edge) {
        float d = query.posTo - query.posFrom;
        if (d < 0.0f) d = -d;
        float total = d * weight + len;
        if (total < bestLen) {
            bestLen = total;
            return true;
        }
        return false;
    }

    int depth = ePath.nElem;
    if (depth >= 4) {
        ePath.ShrinkResize(depth - 1); // depth limit reached: pop and fail
        return false;
    }

    // Gather candidate next edges with their traversal weight and entry position.
    static float lengthEdgeFlip = 0.125f;
    dsVECTOR<hoEDGE *, 8> candEdges;
    dsVECTOR<float, 8>    candWeights;
    dsVECTOR<float, 8>    candLens;

    // Flip to the adjacent (opposite-side) edge.
    if (query.allowFlip && eFrom->adjacentEdge) {
        if (query.is3D && IsValidEdge(query)) {
            candEdges.PushBack(eFrom->adjacentEdge);
            candWeights.PushBack(lengthEdgeFlip);
            float flipLen = eFrom->length - query.posFrom;
            candLens.PushBack(flipLen);
        }
    }

    // Slide along the edge onto its left/right neighbour edges.
    if (query.allowMove) {
        float remLen = eFrom->length - query.posFrom;
        if (remLen < 0.0f) remLen = -remLen;
        float wLeft  = query.posFrom * weight + 0.25f; // neighborEdges1 (left) weight
        float wRight = remLen * weight + 0.25f;        // neighborEdges2 (right) weight
        if (query.is3D) { float t = wLeft; wLeft = wRight; wRight = t; }

        for (int i = 0; i < eFrom->neighborEdges1.nElem; ++i) {
            if (eFrom->neighborEdges1[i] && (eFrom->itrc_st1.state & 0x12)) {
                candEdges.PushBack(eFrom->neighborEdges1[i]);
                candWeights.PushBack(wLeft);
                float el = query.is3D ? 0.0f : eFrom->neighborEdges1[i]->length;
                candLens.PushBack(el);
            }
        }
        for (int i = 0; i < eFrom->neighborEdges2.nElem; ++i) {
            if (eFrom->neighborEdges2[i] && (eFrom->itrc_st2.state & 0x12)) {
                candEdges.PushBack(eFrom->neighborEdges2[i]);
                candWeights.PushBack(wRight);
                float el = query.is3D ? eFrom->neighborEdges2[i]->length : 0.0f;
                candLens.PushBack(el);
            }
        }
    }

    // Explore each candidate on a fresh copy of the path-so-far; adopt the first that succeeds.
    dsVECTOR<hoEDGE *, 8> pathSoFar(ePath);
    bool found = false;
    int nCand = candEdges.nElem;
    for (int k = 0; k < nCand; ++k) {
        dsVECTOR<hoEDGE *, 8> recPath(pathSoFar);
        float candWeight = candWeights[k];
        float candLen    = candLens[k];
        if (candWeight + len < bestLen) {
            HO_EDGE_PATH_QUERY sub;
            sub.itrc        = query.itrc;
            sub.is3D        = query.is3D;
            sub.eFrom       = candEdges[k];
            sub.posFrom     = candLen;
            sub.eTo         = query.eTo;
            sub.posTo       = query.posTo;
            sub.havePrefDir = query.havePrefDir;
            sub.prefDir     = query.prefDir;
            sub.allowMove   = query.allowMove;
            sub.allowFlip   = query.allowFlip;
            sub.allowOuter  = query.allowOuter;
            if (BuildHOEdgePathRec(sub, recPath, candWeight + len, bestLen)) {
                found = true;
                ePath = recPath;
            }
        }
    }

    if (found)
        return true;

    if (ePath.nElem)
        ePath.ShrinkResize(ePath.nElem - 1); // no candidate worked: pop this edge
    return false;
}

#pragma once
#include "../ds/dsSTRID.h"
#include "../ds/dsVECTOR.h"
#include "../ap/apSTATE_T.h"
#include "../m3d/m3dMATR.h"
#include "hoEDGE_DBG_INFO.h"
// ws-engine ho subsystem: a single high-obstacle (HO) edge. DB-verified (types_members hoEDGE),
// total size 0x140: hoEDGE_DBG_INFO base@0 (0x98), then the edge topology, neighbour vectors,
// adjacency links and cached local->world matrix.

struct hoOBJECT;   // ho — owning object   boundary (ptr)
struct m3dV;

struct hoEDGE : hoEDGE_DBG_INFO {
    dsSTRID edgeType;                       // 0x98
    int     id1;                            // 0x9C
    int     id2;                            // 0xA0
    int     idFace;                         // 0xA4
    float   length;                         // 0xA8 — edge arc length
    apSTATE_T<unsigned char>  state;        // 0xAC
    apSTATE_T<unsigned short> itrc_st1;     // 0xAE
    apSTATE_T<unsigned short> itrc_st2;     // 0xB0
    float   posLeft;                        // 0xB4
    float   posRight;                       // 0xB8
    float   heightFloor;                    // 0xBC
    hoOBJECT *owner;                        // 0xC0
    dsVECTOR<hoEDGE *, 8> neighborEdges1;   // 0xC4
    dsVECTOR<hoEDGE *, 8> neighborEdges2;   // 0xD8
    hoEDGE *adjacentEdge;                   // 0xEC
    hoEDGE *edgeChangeRight;                // 0xF0
    hoEDGE *edgeChangeLeft;                 // 0xF4
    int     id_edge;                        // 0xF8
    m3dMATR matrLTCache;                    // 0xFC
    bool    isMatrLTCached;                 // 0x13C

    // Compute the snap-plane normal of this edge into `out`. Body external (ho subsystem). boundary.
    void GetSnapNorm(m3dV *out); // boundary
};

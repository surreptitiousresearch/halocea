#pragma once
#include "../ds/dsVECTOR.h"
#include "../ds/dsFLAGS.h"
#include "../m3d/m3dV.h"
#include "navWP.h"

// ws-engine nav subsystem. DB-verified layouts.
//   navWP_DATA (size 72): a single waypoint entry along a path.
//   navPATH   (size 24): a built path = list of navWP_DATA + reach distance.

#include "WP_DATA_ST.h"      // nav — per-waypoint-datum state flags (DB types_enum_values)
struct navGPH_CELL_DATA;     // nav — graph cell edge data          boundary (ptr)

struct navWP_DATA {
    navWP         wp;                  // 0x00 embedded waypoint (DB: navWP, 44)
    short         wpid;                // 0x2C
    short         wpidParent;          // 0x2E
    dsFLAGS<WP_DATA_ST, unsigned char> state; // 0x30
    short         nsid;                // 0x32
    short         plidFrom;            // 0x34
    short         plidTo;              // 0x36
    float         distKeepAside;       // 0x38
    navGPH_CELL_DATA *edgeData;        // 0x3C
    unsigned short flagsDisabledEdge;  // 0x40
    unsigned int   flagsBlockedWP;     // 0x44
};

struct navPATH {
    dsVECTOR<navWP_DATA, 8> pathWP; // 0x00
    float                   distReach; // 0x14

    navPATH();
    ~navPATH();
    void  Clear();
    // LengthMeters/BuildPath live on navSYS/navPATH; declared where used.  boundary
    float LengthMeters(int from, const void *ctx) const;

    // World "effective" position of waypoint `idx` (writes `out`).  boundary.
    void  GetEffectivePos(int idx, m3dV *out) const;

    // ?GetWP_ID@navPATH@@... — the waypoint id (short) of path entry `idx`.  boundary.
    short GetWP_ID(int idx) const;
    // ?GetEdgeFlags@navPATH@@... — write path entry `idx`'s disabled-edge flags into `edgeFlags`
    // and its blocked-waypoint flags into `blockedFlags`. Used by aiBRAIN::implMoveToHO.  boundary.
    void  GetEdgeFlags(int idx, unsigned short *edgeFlags, unsigned int *blockedFlags) const;
};

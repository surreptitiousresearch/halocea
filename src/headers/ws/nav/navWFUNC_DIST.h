#pragma once
#include "navWFUNC.h"
#include "../m3d/m3dV.h"
#include "../m3d/m3dSPL_LINEAR1D.h"
// ws-engine nav: distance-based waypoint weighting functor. DB-verified layout (types_members
// navWFUNC_DIST = navWFUNC_CENTER + navWFUNC_SQAS):
//   navWFUNC_CENTER@0 (navWFUNC base@0, center@4), navWFUNC_SQAS@0x10 (squaredSpline).  boundary.

// navWFUNC_CENTER — a navWFUNC anchored at a reference `center`. DB size 16.
struct navWFUNC_CENTER : navWFUNC {
    const m3dV center; // 0x04 reference point
};

// navWFUNC_SQAS — squared-distance-along-spline weighting. DB size 20.
struct navWFUNC_SQAS {
    m3dSPL_LINEAR1D squaredSpline; // 0x00

    // 0x8320CD90 (?GetWeight_D2@navWFUNC_SQAS@@QBAMM@Z) — weight for a given squared distance.
    float GetWeight_D2(float distSq) const;
};

struct navWFUNC_DIST : navWFUNC_CENTER, navWFUNC_SQAS {
};

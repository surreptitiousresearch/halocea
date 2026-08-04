#pragma once
#include "../m3d/m3dV.h"
// ws-engine (Saber) cam subsystem: point-of-view quality/blend state (camera-axis history
// used for interest-point smoothing).
// DB-verified layout (types_members camPOV_QUAL): dCam@0 (float), dCamProd@4 (float[5]),
// axisCam@24 (m3dV[4]), axisCamPerp@72 (m3dV[5]), orgCam@132 (m3dV) — size 144.
// Not touched by camCAMERA::SetAngleHor/AdjustMatrW2C/RecomputeViewport/SetParameters —
// modeled here only to keep camCAMERA's layout DB-exact; methods not decompiled.

typedef struct camPOV_QUAL {
    float dCam;
    float dCamProd[5];
    m3dV  axisCam[4];
    m3dV  axisCamPerp[5];
    m3dV  orgCam;
} camPOV_QUAL;

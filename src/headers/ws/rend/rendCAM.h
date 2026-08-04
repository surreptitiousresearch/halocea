#pragma once
#include "../ap/apSTATE_T.h"
#include "../cam/camCAMERA.h"
// ws-engine rend: one per-camera render record held in rendCAM_LIST::list (50 of them).
// DB-verified layout (types_members rendCAM) — size 940. Per the project's boundary conventions
// only the members the cull/validate pass reads are typed; the depth-bias/cull-mode fields
// between distVisFactor and cam are padding at their exact DB offset/size.

typedef struct rendCAM {
    apSTATE_T<unsigned long> state;             // 0x00 — bit4=color cam, bit5/2/11/12/14 flags
    float                    maxDist;           // 0x04
    float                    maxDistExt;        // 0x08
    int                      sourceColorCamIdx; // 0x0C
    int                      sizeFactor;        // 0x10
    float                    distVisFactor;     // 0x14
    short                    lightID;           // 0x18
    short                    lightSplitMask;    // 0x1A
    char                     condRend0;         // 0x1C
    char                     condRend1;         // 0x1D
    char                     originID;          // 0x1E — index into cullInfo.camDistList / origins
    char                     idx;               // 0x1F
    float                    depthBiasConst;    // 0x20
    float                    depthBiasSlope;    // 0x24
    unsigned char            cullDyn;           // 0x28 (vidPASS_RENDERSTATE::CULLMODE)
    unsigned char            cullStat;          // 0x29
    unsigned char            _pad2A[2];         // 0x2A alignment before cam
    camCAMERA                cam;               // 0x2C (896) — frustum + world/view matrices
} rendCAM;

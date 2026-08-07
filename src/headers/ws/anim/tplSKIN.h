#pragma once
#include "../ds/dsFLAGS.h"
#include "../ds/dsVECTOR.h"
#include "../m3d/m3dMATR.h"
// ws-engine tpl: skin binding for an animation template. Full DB-verified layout
// (types_members tplSKIN) — size 96 (0x60).

struct cdtSKEL;        // collision skeleton (pointer only)  boundary
#include "TPL_SKIN_STATE.h" // tpl subsystem — skin state-flag enum (DB types_enum_values)

typedef struct tplSKIN {
    struct CAPSULE_2_VERTICES;      // nested tplSKIN::CAPSULE_2_VERTICES (pointer only)  boundary

    CAPSULE_2_VERTICES *pCdtInfo;                    // 0x00
    cdtSKEL            *pCdtSkel;                    // 0x04 collision skeleton
    dsFLAGS<enum TPL_SKIN_STATE, int> state;         // 0x08
    dsVECTOR<short, 8>  skinCompoundList;            // 0x0C
    dsVECTOR<int, 8>    skinCompoundBoneSearchIndex; // 0x20
    dsVECTOR<short, 8>  skinCompoundBoneIndices;     // 0x34
    int                 nBones;                      // 0x48 number of skin bones
    dsVECTOR<m3dMATR, 16> boneInvBindMatrList;       // 0x4C
} tplSKIN; // 96 bytes

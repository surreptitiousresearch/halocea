#pragma once
#include "../ds/dsSTRID.h"
#include "../m3d/m3dV.h"
// Boundary surface for aiWATCHER::UpdateHeights (a8_plan_wah.cpp): the skeleton bone-name globals,
// the identity matrix, and the file-local height helper.

struct animINST; // ../anim/animINST.h
struct cdtBONE;  // ../cdt/cdtBONE.h
struct m3dMATR;  // ../m3d/m3dMATR.h

// cdt bone-name id globals (const dsSTRID) used to look up the head / stomach bones.
extern const dsSTRID PART_HEAD;    // ?PART_HEAD@@3VdsSTRID@@B    @ 0x842D9A3C
extern const dsSTRID PART_STOMACH; // ?PART_STOMACH@@3VdsSTRID@@B @ 0x842D9A68

// Global identity 4x3 affine matrix (?m3dMatrIdentity@@3Vm3dMATR@@A).
extern m3dMATR m3dMatrIdentity;

// 0x8328D658 — height (along `dirUp`) from `posStart` to `bone`'s world centre within `inst`,
// floored at 0.2. In the binary this is a file-local helper in an anonymous namespace; declared here
// as a free function so the split translation units share it.
// REVERSED: src/ws/ai/CalcYHeightFromMatrToBone.cpp
float CalcYHeightFromMatrToBone(animINST *inst, const m3dV &posStart, const m3dV &dirUp,
                                const cdtBONE *bone);

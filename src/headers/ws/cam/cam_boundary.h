#pragma once
#include "../m3d/m3dMATR.h"
// External cam/m3d free functions referenced by the reversed camCAMERA methods but not
// themselves reversed in this batch — declared here as boundaries.

// In-place 4x4 matrix inverse (?_camMatrInvert style free function, DB name _camMatrInvert /
// symbol camMatrInvert). Called by camCAMERA::AdjustMatrW2C with pMat == &this->matrW2C
// (verified via disasm_func: r3 = this+0x40 at the call site).
void camMatrInvert(m3dMATR *pMat); // boundary

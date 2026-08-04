#include "../../headers/ws/m3d/m3dMATR.h"

// 0x827026CC  _camMatrInvert
// Invert a rigid (rotation + translation) camera matrix in place. The rotation part is inverted
// by transpose (its 3x3 upper-left is orthonormal), the translation is negated and composed on
// the transposed rotation:  pMat = Translate(-t) * transpose(R).
void camMatrInvert(m3dMATR *pMat)
{
    // Transpose of the 3x3 rotation, translation row cleared.
    m3dMATR rotInv;
    rotInv.elements[3]  = 0.0f;
    rotInv.elements[7]  = 0.0f;
    rotInv.elements[11] = 0.0f;
    rotInv.elements[0]  = pMat->elements[0];
    rotInv.elements[1]  = pMat->elements[4];
    rotInv.elements[2]  = pMat->elements[8];
    rotInv.elements[4]  = pMat->elements[1];
    rotInv.elements[5]  = pMat->elements[5];
    rotInv.elements[6]  = pMat->elements[9];
    rotInv.elements[8]  = pMat->elements[2];
    rotInv.elements[9]  = pMat->elements[6];
    rotInv.elements[10] = pMat->elements[10];
    // Clear the translation row (v[3] = elements[12..14]); elements[15] = 1.
    rotInv.elements[12] = 0.0f;
    rotInv.elements[13] = 0.0f;
    rotInv.elements[14] = 0.0f;
    rotInv.elements[15] = 1.0f;

    m3dMATR negTranslate;
    negTranslate.Identity();
    // oper is a spilled/undefined scalar in the decompile (r4, never initialised); it selects the
    // Translate compose mode. Preserved verbatim as the decompiler modelled it.
    int oper; // matches decompiler's uninitialised r4
    negTranslate.Translate(-pMat->elements[12], -pMat->elements[13], -pMat->elements[14], oper);

    m3dMultiplyMatr(&negTranslate, &rotInv, pMat);
}

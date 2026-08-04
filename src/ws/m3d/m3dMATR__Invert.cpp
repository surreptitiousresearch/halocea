#include <math.h>
#include "m3dMATR.h"

// m3dMATR::Invert @ 0x82544630
// Full 4x4 inverse by the adjugate (cofactor) method. Returns 0 (leaving destMatr untouched)
// when the matrix is singular, i.e. |determinant| < 1e-14; otherwise writes the inverse into
// destMatr and returns 1. Each inverse element is a signed 3x3 cofactor times 1/determinant.
// The determinant is expanded along the bottom row using its four 3x3 minors.
int m3dMATR::Invert(m3dMATR *destMatr) const
{
    // Reused 2x2 sub-determinants from the top three rows.
    float minorA = elements[7] * elements[2] - elements[3] * elements[6];
    float minorB = elements[6] * elements[1] - elements[2] * elements[5];
    float minorC = elements[0] * elements[5] - elements[1] * elements[4];

    // 3x3 minors of the bottom-row elements (col 0..3) — also the last inverse column.
    float minorRow3Col0 = elements[11] * minorB
        + (elements[9] * minorA - elements[10] * (elements[7] * elements[1] - elements[3] * elements[5]));
    float minorRow3Col1 = elements[11] * (elements[6] * elements[0] - elements[2] * elements[4])
        + (elements[8] * minorA - elements[10] * (elements[7] * elements[0] - elements[3] * elements[4]));
    float minorRow3Col2 = elements[11] * minorC
        + (elements[8] * (elements[7] * elements[1] - elements[3] * elements[5])
           - elements[9] * (elements[7] * elements[0] - elements[3] * elements[4]));
    float minorRow3Col3 = elements[10] * minorC
        + (elements[8] * minorB - elements[9] * (elements[6] * elements[0] - elements[2] * elements[4]));

    float determinant = elements[15] * minorRow3Col3
        - (elements[14] * minorRow3Col2
           - (elements[13] * minorRow3Col1 - elements[12] * minorRow3Col0));

    if (fabsf(determinant) < 1.0e-14f)
        return 0;

    float invDet = 1.0f / determinant;

    // Cofactor numerators for the remaining inverse elements.
    float cof00 = elements[11] * (elements[5] * elements[14] - elements[6] * elements[13])
        + (elements[9] * (elements[6] * elements[15] - elements[7] * elements[14])
           - elements[10] * (elements[5] * elements[15] - elements[7] * elements[13]));
    float cof01 = elements[11] * (elements[14] * elements[1] - elements[2] * elements[13])
        + (elements[9] * (elements[15] * elements[2] - elements[3] * elements[14])
           - elements[10] * (elements[15] * elements[1] - elements[3] * elements[13]));
    float cof02 = elements[15] * minorB
        + (elements[13] * minorA - elements[14] * (elements[7] * elements[1] - elements[3] * elements[5]));

    float cof10 = elements[11] * (elements[4] * elements[14] - elements[6] * elements[12])
        + (elements[8] * (elements[6] * elements[15] - elements[7] * elements[14])
           - elements[10] * (elements[4] * elements[15] - elements[7] * elements[12]));
    float cof11 = elements[11] * (elements[14] * elements[0] - elements[2] * elements[12])
        + (elements[8] * (elements[15] * elements[2] - elements[3] * elements[14])
           - elements[10] * (elements[15] * elements[0] - elements[3] * elements[12]));
    float cof12 = elements[15] * (elements[6] * elements[0] - elements[2] * elements[4])
        + (elements[12] * minorA - elements[14] * (elements[7] * elements[0] - elements[3] * elements[4]));

    float cof20 = elements[11] * (elements[4] * elements[13] - elements[5] * elements[12])
        + (elements[8] * (elements[5] * elements[15] - elements[7] * elements[13])
           - elements[9] * (elements[4] * elements[15] - elements[7] * elements[12]));
    float cof21 = elements[11] * (elements[13] * elements[0] - elements[1] * elements[12])
        + (elements[8] * (elements[15] * elements[1] - elements[3] * elements[13])
           - elements[9] * (elements[15] * elements[0] - elements[3] * elements[12]));
    float cof22 = elements[15] * minorC
        + (elements[12] * (elements[7] * elements[1] - elements[3] * elements[5])
           - elements[13] * (elements[7] * elements[0] - elements[3] * elements[4]));

    float cof30 = elements[10] * (elements[4] * elements[13] - elements[5] * elements[12])
        + (elements[8] * (elements[5] * elements[14] - elements[6] * elements[13])
           - elements[9] * (elements[4] * elements[14] - elements[6] * elements[12]));
    float cof31 = elements[10] * (elements[13] * elements[0] - elements[1] * elements[12])
        + (elements[8] * (elements[14] * elements[1] - elements[2] * elements[13])
           - elements[9] * (elements[14] * elements[0] - elements[2] * elements[12]));
    float cof32 = elements[14] * minorC
        + (elements[12] * minorB - elements[13] * (elements[6] * elements[0] - elements[2] * elements[4]));

    destMatr->elements[0]  = -cof00 * invDet;
    destMatr->elements[1]  =  cof01 * invDet;
    destMatr->elements[2]  =  cof02 * invDet;
    destMatr->elements[3]  = -minorRow3Col0 * invDet;

    destMatr->elements[4]  =  cof10 * invDet;
    destMatr->elements[5]  = -cof11 * invDet;
    destMatr->elements[6]  = -cof12 * invDet;
    destMatr->elements[7]  =  minorRow3Col1 * invDet;

    destMatr->elements[8]  = -cof20 * invDet;
    destMatr->elements[9]  =  cof21 * invDet;
    destMatr->elements[10] =  cof22 * invDet;
    destMatr->elements[11] = -minorRow3Col2 * invDet;

    destMatr->elements[12] =  cof30 * invDet;
    destMatr->elements[13] = -cof31 * invDet;
    destMatr->elements[14] = -cof32 * invDet;
    destMatr->elements[15] =  minorRow3Col3 * invDet;

    return 1;
}

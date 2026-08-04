#include <math.h>
#include "m3dV.h"
#include "m3dMATR.h"
#include "m3d_boundary.h"

// m3dMATR::Check @ 0x82546188
// Validate the matrix: returns 0 if any of the 16 elements is non-finite, else 1. When the
// three basis rows share a (near-)uniform scale it also normalises copies of the axes and
// measures the inter-axis angles; those calls are debug-side-effect assertions only and do
// not affect the result. A final pass re-validates the 3x3 basis by rows and by columns.
int m3dMATR::Check() const
{
    for (int i = 0; i < 16; ++i)
        if (!_m3dCheckValid(elements[i]))
            return 0;

    float scaleX, scaleY, scaleZ;
    GetScale(&scaleX, &scaleY, &scaleZ);

    if (fabsf(scaleX - scaleY) < 0.000099999997f && fabsf(scaleY - scaleZ) < 0.000099999997f)
    {
        m3dV axisX = { elements[0], elements[1], elements[2] };
        m3dV axisY = { elements[4], elements[5], elements[6] };
        m3dV axisZ = { elements[8], elements[9], elements[10] };
        _m3dNormalize(&axisX);
        _m3dNormalize(&axisY);
        _m3dNormalize(&axisZ);
        m3dAngleVector(&axisX, &axisY);
        m3dAngleVector(&axisX, &axisZ);
        m3dAngleVector(&axisY, &axisZ);
    }

    // Re-validate the basis by rows (row i, columns 0..2)...
    for (int i = 0; i < 12; i += 4)
        for (int j = 0; j < 3; ++j)
            _m3dCheckValid(elements[i + j]);

    // ...and by columns (column c, rows 0..2).
    for (int c = 0; c < 3; ++c)
        for (int r = 0; r < 3; ++r)
            _m3dCheckValid(elements[c + 4 * r]);

    return 1;
}

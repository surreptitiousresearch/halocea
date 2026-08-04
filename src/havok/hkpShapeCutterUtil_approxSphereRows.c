#include <math.h>

/* hkpShapeCutterUtil::approxSphereRows
   Pick a UV-sphere row count so that each latitude band is about `edgeSize`
   across, clamped so the resulting rows*rows grid stays under `maxFaces`.
   DEVIATION: a prior reconstruction carried a phantom 4th `maxVertices`
   parameter; the binary (?approxSphereRows@hkpShapeCutterUtil@@SAHMHM@Z,
   0x83F948D0) takes exactly (f1 edgeSize, r4 maxFaces, f2 radius) and the r4
   cap is the value compared against rows*rows. */
int hkpShapeCutterUtil_approxSphereRows(float edgeSize, int maxFaces, float radius)
{
    int rows;

    /* half-circumference / edgeSize, rounded to nearest */
    rows = (int)((radius * 3.1415927f) / edgeSize + 0.5f);
    if (rows < 2)
        return 2;

    if (rows * rows > maxFaces)
    {
        /* DEVIATION: PPC __fsqrts reconstructed as sqrtf over the cap */
        rows = (int)sqrtf((float)maxFaces);
        if (rows < 2)
            return 2;
    }
    return rows;
}

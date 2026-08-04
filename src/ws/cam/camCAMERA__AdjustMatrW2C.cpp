#include "../../headers/ws/cam/camCAMERA.h"
#include "../../headers/ws/cam/cam_boundary.h"
#include "../../headers/ws/m3d/m3d_boundary.h"
#include <math.h>

// 0x82702A88 — private. Normalizes matrC2W's 3 basis-axis rows (right/up/forward) in
// place, copies the result into matrW2C, then inverts matrW2C in place.
void camCAMERA::AdjustMatrW2C()
{
    for (int axisRow = 0; axisRow < 3; axisRow++)
    {
        float *axisRowElements = &matrC2W.elements[axisRow * 4];

        float axisLengthSquared = 0.0f;
        for (int component = 0; component < 3; component++)
        {
            float axisComponent = axisRowElements[component];
            _m3dCheckValid(axisComponent);
            axisLengthSquared = axisComponent * axisComponent + axisLengthSquared;
        }
        _m3dCheckValid(axisLengthSquared);

        float invAxisLength = 1.0f / sqrtf(axisLengthSquared); // disasm: fsqrts (single-precision)

        axisRowElements[0] *= invAxisLength;
        axisRowElements[1] *= invAxisLength;
        axisRowElements[2] *= invAxisLength;
    }

    // matrW2C is a straight copy of the now axis-normalized matrC2W, inverted in place.
    // Disasm shows this as 8 back-to-back QWORD (2-float) load/store pairs covering the
    // full 64-byte matrix; collapsed here to a whole-struct assignment (m3dMATR is POD).
    matrW2C = matrC2W;
    camMatrInvert(&matrW2C); // disasm: r3 = this+0x40 (&matrW2C) at the call site
}

#include "../../headers/ws/ai/aiGOAL_BASE.h"
#include "../../headers/ws/m3d/m3dSPL_LINEAR1D.h"
#include "../../headers/ws/m3d/m3dSPL.h"
#include <math.h>

// aiGOAL_BASE::IsEqualSplines(const m3dSPL_LINEAR1D&, const m3dSPL_LINEAR1D&, float) @ 0x8329B380
// ?IsEqualSplines@aiGOAL_BASE@@SA_NABVm3dSPL_LINEAR1D@@0M@Z
//
// Static: true when the two piecewise-linear splines agree to `precision` at every keyframe
// argument of either. Walks the union of both splines' knot arguments in ascending order; at each
// step the owning spline contributes its keyframe value directly (GetKpValue) while the other is
// evaluated at that argument (GetValue). Any pair differing by more than `precision` fails.
bool aiGOAL_BASE::IsEqualSplines(const m3dSPL_LINEAR1D &spline1,
                                 const m3dSPL_LINEAR1D &spline2, float precision)
{
    const int n1 = spline1.nKp;
    const int n2 = spline2.nKp;
    if (n1 < 1 || n2 < 1)
        return false;

    // m3dSPL vtable GetValue(self, t, out, aux): evaluate the "other" spline at argument t.
    // (self upcasts m3dSPL_LINEAR1D* -> m3dSPL*; aux is unused here — see disasm 0x82A29710.)
    m3dSPL_LINEAR1D &s1 = const_cast<m3dSPL_LINEAR1D &>(spline1);
    m3dSPL_LINEAR1D &s2 = const_cast<m3dSPL_LINEAR1D &>(spline2);

    int i = 0, j = 0;
    for (;;)
    {
        const float a1 = (i < n1) ? s1.GetKpArg(i) : 2000000.0f;
        const float a2 = (j < n2) ? s2.GetKpArg(j) : 2000000.0f;

        float val1 = 2000000.0f, val2 = 2000000.0f;
        if (a1 < a2)
        {
            // knot belongs to spline1; evaluate spline2 at that argument
            s1.GetKpValue(i, &val1);
            s2.__vftable->GetValue(&s2, a1, &val2, nullptr);
            ++i;
        }
        else if (a2 < a1)
        {
            // knot belongs to spline2; evaluate spline1 at that argument
            s1.__vftable->GetValue(&s1, a2, &val1, nullptr);
            s2.GetKpValue(j, &val2);
            ++j;
        }
        else
        {
            // shared knot argument
            s1.GetKpValue(i, &val1);
            s2.GetKpValue(j, &val2);
            ++i;
            ++j;
        }

        if (fabsf(val2 - val1) > precision)
            return false;
        if (i >= n1 && j >= n2)
            return true;
    }
}

#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiDBG_SPLINE_POINT.h"
#include <math.h>

// aiCOORDINATOR::BuildGroupWPSetCurveMesh @ 0x83236BA8
// ?BuildGroupWPSetCurveMesh@aiCOORDINATOR@@IAAXABV?$dsVECTOR@Um2dV@@$07@@ABVm3dMATR@@@Z
//
// Build the group's WP-set weighting-curve debug mesh: a surface of revolution around the group
// axis. Each 2D curve point (param, weight) is coloured by lerping ColorMin->ColorMax over the
// [ValueMin, ValueMax] weight range and lifted to a 3D spline point; the spline is then swept
// through NSeg (clamped 3..128) rotations about Y (each composed with startMatr) to produce the
// revolved vertex ring, and a triangle-strip-style index buffer stitches adjacent rings.
void aiCOORDINATOR::BuildGroupWPSetCurveMesh(const dsVECTOR<m2dV, 8> &curvePoints,
                                             const m3dMATR &startMatr)
{
    dbgGroupWPSetCurveVBuf.nElem = 0;
    dbgGroupWPSetCurveIBuf.nElem = 0;

    int nPts = curvePoints.nElem;

    dsVECTOR<aiDBG_SPLINE_POINT, 8> splinePts;
    aiDBG_SPLINE_POINT *pData = nullptr;
    int nSpline = 0;

    const float rMin = dbg_isCoordGroupCurve_ColorMin.value.r;
    const float gMin = dbg_isCoordGroupCurve_ColorMin.value.g;
    const float bMin = dbg_isCoordGroupCurve_ColorMin.value.b;
    const float aMin = dbg_isCoordGroupCurve_ColorMin.value.a;
    const float rMax = dbg_isCoordGroupCurve_ColorMax.value.r;
    const float gMax = dbg_isCoordGroupCurve_ColorMax.value.g;
    const float bMax = dbg_isCoordGroupCurve_ColorMax.value.b;
    const float aMax = dbg_isCoordGroupCurve_ColorMax.value.a;
    const float valueMin = dbg_isCoordGroupCurve_ValueMin.value;
    const float valueMax = dbg_isCoordGroupCurve_ValueMax.value;

    if (nPts > 0)
    {
        const float range = valueMax - valueMin;
        for (int i = 0; i < nPts; ++i)
        {
            const m2dV &pt = curvePoints[i];
            float clamped = pt.y;
            if (clamped <= valueMin) clamped = valueMin;
            if (clamped >= valueMax) clamped = valueMax;

            float num = clamped - valueMin;
            // Faithful to the binary: the normalising factor starts as (valueMax - valueMin) and is
            // replaced by 1/range only when range is non-negligible.
            float factor = valueMax - valueMin;
            if (fabsf(range) > 0.000001f)
                factor = 1.0f / range;
            float t = factor * num;

            aiDBG_SPLINE_POINT sp;
            sp.pos.x = pt.x;
            sp.pos.y = pt.y * dbg_isCoordGroupCurve_VScale.value;
            sp.pos.z = m3dVZero.z;

            int rB = (int)((1.0f - t) * rMin + rMax * t);
            int gB = (int)((1.0f - t) * gMin + gMax * t);
            int bB = (int)((1.0f - t) * bMin + bMax * t);
            int aB = (int)((1.0f - t) * aMin + aMax * t);
            sp.color = (((((unsigned)aB << 8) | (unsigned char)rB) << 8
                        | (unsigned char)gB) << 8) | (unsigned char)bB;

            splinePts.PushBack(sp);
        }
        nSpline = splinePts.nElem;
        pData = splinePts.pData;
    }

    int nSeg = dbg_isCoordGroupCurve_NSeg.value;
    if (nSeg < 3)
        nSeg = 3;
    else if (nSeg > 128)
        nSeg = 128;

    for (int seg = 0; seg < nSeg; ++seg)
    {
        m3dMATR mat;
        mat.MakeRotY((float)(seg * (360.0f / nSeg)));
        mat.Transform(&startMatr, 2);

        for (int k = 0; k < nSpline; ++k)
        {
            aiDBG_SPLINE_POINT *sp = &pData[k];
            aiDBGSHAPE_VERT vert;
            vert.v.x = sp->pos.x;
            vert.v.y = sp->pos.y;
            vert.v.z = sp->pos.z;
            vert.color = sp->color;
            mat.TransformPoint(&vert.v);
            dbgGroupWPSetCurveVBuf.PushBack(vert);
        }
    }

    for (int seg = 0; seg < nSeg; ++seg)
    {
        int base = seg * nSpline;
        int col = ((seg + 1) % nSeg) * nSpline;
        for (int k = 0; k < nSpline - 1; ++k)
        {
            unsigned short idx;
            idx = (unsigned short)(base + k);       dbgGroupWPSetCurveIBuf.PushBack(idx);
            idx = (unsigned short)(base + 1 + k);   dbgGroupWPSetCurveIBuf.PushBack(idx);
            idx = (unsigned short)(col + k);        dbgGroupWPSetCurveIBuf.PushBack(idx);
            idx = (unsigned short)(col + k);        dbgGroupWPSetCurveIBuf.PushBack(idx);
            idx = (unsigned short)(base + 1 + k);   dbgGroupWPSetCurveIBuf.PushBack(idx);
            idx = (unsigned short)(col + 1 + k);    dbgGroupWPSetCurveIBuf.PushBack(idx);
        }
    }
}

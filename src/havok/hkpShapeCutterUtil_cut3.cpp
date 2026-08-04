#include "../headers/havok/hkpShape.h"
#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkReferencedObject.h"

struct hkpConvexVerticesShape;

/* The single-shape plane cut (already sourced, src/havok/hkpShapeCutterUtil_cut.cpp):
   returns the (referenced) piece on the negative side of the plane, shapeIn itself
   when the plane misses, or null when the shape is fully cut away. */
extern "C" const hkpConvexVerticesShape *hkpShapeCutterUtil_cut(hkpShape *shapeIn, const hkVector4 *plane,
                                                                float extraConvexRadiusForImplicitShapes);

/* hkpShapeCutterUtil::cut (0x83F97D40) — the two-sided convenience overload:
   split shapeIn by planeIn, delivering the positive-side piece in positiveOut and
   the negative-side piece in negativeOut (obtained by cutting again with the
   flipped plane). When the plane does not actually divide the shape the returned
   piece is shapeIn itself, in which case there is no opposite piece; when nothing
   is cut, shapeIn is handed back (with an added reference) as the sole piece.
   DEVIATION: VMX128 plane negation reconstructed as scalar per-lane negation.
   DEVIATION: this is the two-out-parameter DB overload of cut, flattened to the
   distinct C name cut3 to avoid colliding with the single-shape overload's flat
   name. DEVIATION: a prior reconstruction carried a phantom `remainderOut`
   parameter (decompiler artifact); the binary (0x83F97D40) takes exactly 5 args
   (r3 shapeIn, r4 planeIn, f1 extraConvexRadius, r6 positiveOut, r7 negativeOut)
   and both inner cut calls are the 3-arg single-shape overload. */
void hkpShapeCutterUtil_cut3(const hkpShape *shapeIn, const hkVector4 *planeIn,
                             float extraConvexRadius,
                             const hkpShape **positiveOut,
                             const hkpShape **negativeOut)
{
    const hkpShape *positive = (const hkpShape *)hkpShapeCutterUtil_cut((hkpShape *)shapeIn, planeIn, extraConvexRadius);
    *positiveOut = positive;

    if (positive)
    {
        if (positive == shapeIn)
        {
            *negativeOut = nullptr;
        }
        else
        {
            hkVector4 flippedPlane;
            int lane;
            for (lane = 0; lane < 4; ++lane)
                flippedPlane.m_quad.___u0.vector4_f32[lane] = -planeIn->m_quad.___u0.vector4_f32[lane];
            *negativeOut = (const hkpShape *)hkpShapeCutterUtil_cut((hkpShape *)shapeIn, &flippedPlane, 0.001f);
        }
    }
    else
    {
        hkReferencedObject_addReference((hkReferencedObject *)shapeIn);
        *negativeOut = shapeIn;
    }
}

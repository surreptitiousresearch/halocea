/* ?castRay@hkpShape@@QBA?AVhkBool@@ABUhkpShapeRayCastInput@@AAUhkpShapeRayCastOutput@@@Z @0x82EB78A8 */
#include "../headers/havok/hkpShapeVtbl.h"

/* hkpShape::castRay — non-virtual forwarder to the shape's virtual castRayImpl
   (vtable slot 0x20), returning the hkBool hit result by value. DEVIATION: on
   PPC the hkBool return is an sret in r3 and the shape is r4; IDA rendered the
   sret buffer as the first named parameter and loaded the vtable from it. Here
   the dispatch is modelled straight: vtable comes from `self`, the caller's
   result buffer is threaded through and returned. */
hkBool *hkpShape_castRay(hkpShape *self, hkBool *result,
                         const hkpShapeRayCastInput *input,
                         hkpShapeRayCastOutput *output)
{
    HKP_SHAPE_VTBL(self)->castRayImpl(self, result, input, output);
    return result;
}

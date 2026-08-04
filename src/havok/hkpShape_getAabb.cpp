#include "../headers/havok/hkpShapeVtbl.h"

/* hkpShape::getAabb — non-virtual forwarder to the shape's virtual getAabbImpl
   (vtable slot 0x1C). DEVIATION: the real call passes (self, localToWorld, out,
   tolerance); IDA's vtbl slot lists tolerance before out. */
extern "C" void hkpShape_getAabb(hkpShape *self, const hkTransform *localToWorld,
                      float tolerance, hkAabb *out)
{
    HKP_SHAPE_VTBL(self)->getAabbImpl(self, localToWorld, out, tolerance);
}

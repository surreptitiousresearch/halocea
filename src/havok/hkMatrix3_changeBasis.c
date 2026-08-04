#include "../headers/havok/hkMatrix3.h"
#include "../headers/havok/hkRotation.h"

/* hkMatrix3::changeBasis — re-express this matrix in the basis given by rotation
   r: this = r * this * r^-1. Implemented via the two library multiplies the
   original calls: tmp = this * r^-1 (setMulInverse), then this = r * tmp (setMul).
   Both callees stay extern (2-level descent boundary). Verified against disasm
   0x838E3B4C/0x838E3B5C: setMul's destination is `this` (the decompiler lost
   that the r6 scratch aliased r3). */
extern void hkMatrix3_setMulInverse(hkMatrix3 *out, const hkMatrix3 *a, const hkMatrix3 *bInv); /* hkMatrix3::setMulInverse */
extern void hkMatrix3_setMul(hkMatrix3 *out, const hkRotation *a, const hkMatrix3 *b);           /* hkMatrix3::setMul */

void hkMatrix3_changeBasis(hkMatrix3 *self, const hkRotation *r)
{
    hkMatrix3 tmp;
    hkMatrix3_setMulInverse(&tmp, self, r);
    hkMatrix3_setMul(self, r, &tmp);
}

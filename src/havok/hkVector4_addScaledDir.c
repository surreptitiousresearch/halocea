/* FUNCTION_INDEX entry: hkVector4_addScaledDir @0x82EB7888 (?addMul4@hkVector4@@QAAXABVhkSimdReal@@ABV1@@Z) */
#include "../headers/havok/hkVector4.h"

/* dst += dir * s  (hkVector4::addMul4, the hkSimdReal overload: fused
   multiply-add of a scaled direction vector). The out-of-line body is five
   instructions — lvx128 of this/scale/dir, one vmaddfp, stvx128 — and the op is
   additionally inlined at most VMX128 call sites; this is the canonical scalar
   definition the frontier callers (decorateConnection) reference through the
   extern. The scale applies to the xyz lanes that Havok direction vectors
   carry; the w lane is carried through by the same fma.
   DEVIATION: original is a single VMX128 vmaddfp of dir by the broadcast scalar
   quad; reconstructed as scalar float, and the ABI order is (this, s, dir) —
   r4 is the hkSimdReal quad and r5 the vector. */
void hkVector4_addScaledDir(hkVector4 *dst, const hkVector4 *dir, float s)
{
    int i;
    for (i = 0; i < 4; ++i)
        dst->m_quad.___u0.v[i] += dir->m_quad.___u0.v[i] * s;
}

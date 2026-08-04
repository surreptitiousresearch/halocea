#include "../headers/havok/hkVector4.h"

/* dst += dir * s  (hkVector4::addMul: fused multiply-add of a scaled
   direction vector). No standalone routine survives in the binary — the
   op is fully inlined at every VMX128 call site (vmaddfp of a splatted
   scalar) — so this is the canonical scalar definition the frontier
   callers (decorateConnection) reference through the extern. The scale
   applies to the xyz lanes that Havok direction vectors carry; the w lane
   is carried through by the same fma. */
void hkVector4_addScaledDir(hkVector4 *dst, const hkVector4 *dir, float s)
{
    int i;
    for (i = 0; i < 4; ++i)
        dst->m_quad.___u0.v[i] += dir->m_quad.___u0.v[i] * s;
}

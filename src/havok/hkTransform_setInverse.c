#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkTransform.h"

/* Deeper callee kept as a boundary extern (2-level descent limit):
   transpose of the 3x3 rotation. */
extern void hkMatrix3_setTranspose(hkMatrix3 *dst, const hkMatrix3 *src); /* hkMatrix3::setTranspose — sig matched to def (hkRotation is-a hkMatrix3, layout-identical) */

/* hkTransform::setInverse — this = t^-1 for a rigid transform:
   this.rotation    = transpose(t.rotation)
   this.translation = transpose(t.rotation) * (-t.translation)
   DEVIATION: original is a setTranspose call followed by VMX128 that xors the
   translation with a sign mask (negate) and multiply-accumulates the three
   (already transposed) columns; reconstructed as scalar float. */
void hkTransform_setInverse(hkTransform *this, const hkTransform *t)
{
    float nx, ny, nz;
    int i;
    hkMatrix3_setTranspose(&this->m_rotation, &t->m_rotation);
    nx = -t->m_translation.m_quad.___u0.v[0];
    ny = -t->m_translation.m_quad.___u0.v[1];
    nz = -t->m_translation.m_quad.___u0.v[2];
    for (i = 0; i < 4; ++i)
        this->m_translation.m_quad.___u0.v[i] =
              this->m_rotation.m_col0.m_quad.___u0.v[i] * nx
            + this->m_rotation.m_col1.m_quad.___u0.v[i] * ny
            + this->m_rotation.m_col2.m_quad.___u0.v[i] * nz;
}

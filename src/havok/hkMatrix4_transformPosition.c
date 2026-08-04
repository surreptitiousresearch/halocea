#include "../headers/havok/hkMatrix4.h"
#include "../headers/havok/hkVector4.h"

/* hkMatrix4::transformPosition — transform a homogeneous position:
   positionOut = col0*x + col1*y + col2*z + col3, where (x,y,z) are the first
   three lanes of positionIn.
   DEVIATION: the original splats x/y/z (vspltw) and accumulates the columns with
   vmulfp128/vmaddfp; reconstructed as a scalar loop over the four lanes. */
void hkMatrix4_transformPosition(hkMatrix4 *this, const hkVector4 *positionIn, hkVector4 *positionOut)
{
    float x = positionIn->m_quad.___u0.v[0];
    float y = positionIn->m_quad.___u0.v[1];
    float z = positionIn->m_quad.___u0.v[2];
    int i;
    for (i = 0; i < 4; ++i)
        positionOut->m_quad.___u0.v[i] =
              this->m_col0.m_quad.___u0.v[i] * x
            + this->m_col1.m_quad.___u0.v[i] * y
            + this->m_col2.m_quad.___u0.v[i] * z
            + this->m_col3.m_quad.___u0.v[i];
}

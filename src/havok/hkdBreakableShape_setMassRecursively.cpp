/* ?setMassRecursively@hkdBreakableShape@@QAAXMM@Z @0x83848C00 */
#include "../headers/havok/hkdBreakableShape.h"
#include "../headers/havok/hkdShapeInstanceInfo.h"
#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkMatrix3.h"
#include "../headers/havok/hkSimdReal.h"

/* Deeper callee kept as a boundary extern (2-level descent limit): scale the
   inertia tensor in place by a broadcast hkSimdReal. */
extern void hkMatrix3_mul(hkMatrix3 *m, const hkSimdReal *scale); /* hkMatrix3::mul(hkSimdReal const&) */

#define HK_MASS_EPSILON 1.1920929e-7f

/* hkdBreakableShape::setMassRecursively — set this shape's mass to newMass,
   rescale its inertia tensor by (newMass/oldMass)*inertiaTensorFactor, then
   distribute the mass across children proportionally to their volumes and
   recurse. */
void hkdBreakableShape_setMassRecursively(hkdBreakableShape *self, float newMass, float inertiaTensorFactor)
{
    float massRatio = newMass / self->m_massProps.m_mass;
    float inertiaScaleValue;
    hkVector4 inertiaScale;
    float totalVolume;
    float invVolume;
    int numChildren;
    int i;

    self->m_massProps.m_mass = newMass;
    inertiaScaleValue = massRatio * inertiaTensorFactor;
    for (i = 0; i < 4; ++i)
        inertiaScale.m_quad.___u0.v[i] = inertiaScaleValue;
    /* inertiaScale is a broadcast scalar (all four lanes equal); reinterpret the
       hkVector4 as the hkSimdReal the callee expects. */
    hkMatrix3_mul(&self->m_massProps.m_inertiaTensor, (const hkSimdReal *)&inertiaScale);

    numChildren = self->m_children.m_size;
    totalVolume = 0.0f;
    for (i = 0; i < numChildren; ++i)
        totalVolume += self->m_children.m_data[i].m_shape->m_massProps.m_volume;

    invVolume = 1.0f / (totalVolume + HK_MASS_EPSILON);
    for (i = 0; i < self->m_children.m_size; ++i)
    {
        hkdBreakableShape *child = (hkdBreakableShape *)self->m_children.m_data[i].m_shape;
        hkdBreakableShape_setMassRecursively(child,
                                             child->m_massProps.m_volume * invVolume * newMass,
                                             inertiaTensorFactor);
    }
}

/* ?computeShapeVolumeMassProperties@hkpInertiaTensorComputer@@SAXPBVhkpShape@@MAAUhkpMassProperties@@@Z @0x83A10108 */
#include "../headers/havok/hkpShape.h"
#include "../headers/havok/hkpMassProperties.h"
#include "../headers/havok/hkTransform.h"
#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkMatrix3.h"
#include "../headers/havok/hkSimdReal.h"
#include <string.h>

/* Deeper per-shape-type volume/mass integrators are boundary externs (this is the
   entry point that seeds the identity transform and normalises the result). */
extern void computeRecursiveShapeVolumeMassProperties(const hkpShape *shape, const hkTransform *transform,
                                                      hkpMassProperties *accumulator);
extern void hkMatrix3_mul(hkMatrix3 *m, const hkSimdReal *scale); /* hkMatrix3::mul(hkSimdReal const&) */

/* hkpInertiaTensorComputer::computeShapeVolumeMassProperties — integrate the
   shape's volume, center of mass and inertia tensor over its geometry at the
   identity transform, then scale mass and inertia so the shape has the requested
   density (mass per unit volume). A zero-volume shape leaves `result` untouched.
   DEVIATION: the decompiler split the single output pointer across two apparent
   parameters ("result" and "a4") and typed density as a double; the canonical
   3-argument form used by every caller — (shape, density, result) — is restored
   here, with the VMX128 zeroing/identity setup expressed as scalar writes. */
void hkpInertiaTensorComputer_computeShapeVolumeMassProperties(const hkpShape *shape, float density,
                                                               hkpMassProperties *result)
{
    hkpMassProperties integrated;
    hkTransform identity;
    int i;

    /* identity transform */
    memset(&identity, 0, sizeof(identity));
    identity.m_rotation.m_col0.m_quad.___u0.v[0] = 1.0f;
    identity.m_rotation.m_col1.m_quad.___u0.v[1] = 1.0f;
    identity.m_rotation.m_col2.m_quad.___u0.v[2] = 1.0f;

    /* zeroed accumulator */
    integrated.m_volume = 0.0f;
    integrated.m_mass = 0.0f;
    memset(&integrated.m_centerOfMass, 0, sizeof(integrated.m_centerOfMass));
    memset(&integrated.m_inertiaTensor, 0, sizeof(integrated.m_inertiaTensor));

    computeRecursiveShapeVolumeMassProperties(shape, &identity, &integrated);

    if (integrated.m_volume != 0.0f)
    {
        float densityScale = density / integrated.m_volume;
        hkSimdReal densityScaleSimd;

        integrated.m_mass = densityScale * integrated.m_mass;
        for (i = 0; i < 4; ++i)
            densityScaleSimd.m_real.___u0.v[i] = densityScale;
        hkMatrix3_mul(&integrated.m_inertiaTensor, &densityScaleSimd);

        result->m_volume = integrated.m_volume;
        result->m_mass = integrated.m_mass;
        result->m_centerOfMass = integrated.m_centerOfMass;
        result->m_inertiaTensor = integrated.m_inertiaTensor;
    }
}

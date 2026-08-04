#include "../headers/havok/hkpMassElement.h"
#include "../headers/havok/hkpMassProperties.h"
#include "../headers/havok/hkMatrix3.h"
#include "../headers/havok/hkVector4.h"
#include "../headers/havok/hkTransform.h"
#include "../headers/havok/hkArray.h"
#include "../headers/havok/hkResult.h"

/* Boundary callees (2-level descent). */
extern void hkVector4_setTransformedPos(hkVector4 *out, const hkTransform *t, const hkVector4 *v); /* hkVector4::setTransformedPos */
extern void hkMatrix3_changeBasis(hkMatrix3 *m, const hkRotation *rotation);                       /* hkMatrix3::changeBasis */
extern void hkMatrix3_add(hkMatrix3 *dst, const hkMatrix3 *src);                                   /* hkMatrix3::add */

#define LANE(v, i) ((v).m_quad.___u0.vector4_f32[(i)])

/* hkpInertiaTensorComputer::combineMassProperties — accumulate the mass, volume,
   centre of mass and inertia tensor of a parent body from an array of child mass
   elements (each a mass property set placed by a transform). Two passes:
     1. Sum mass and volume, and the mass-weighted world centre of mass; the
        combined centre is that sum divided by the total mass.
     2. For each child, rotate its inertia tensor into world space, then apply the
        parallel-axis theorem about the combined centre of mass (add
        m*(|r|^2 I - r (x) r) for the displacement r) and accumulate.
   Returns HK_FAILURE when the total mass is non-positive, HK_SUCCESS otherwise.

   DEVIATION: the DB body is VMX128 (vmaddfp centre accumulation, lane stores);
   reproduced here as scalar lane arithmetic. */
hkResult hkpInertiaTensorComputer_combineMassProperties(const hkArray<hkpMassElement> *elements,
                                                        hkpMassProperties *result)
{
    hkVector4 weightedCentre;
    float totalMass = 0.0f;
    float totalVolume = 0.0f;
    int i;

    LANE(weightedCentre, 0) = 0.0f;
    LANE(weightedCentre, 1) = 0.0f;
    LANE(weightedCentre, 2) = 0.0f;
    LANE(weightedCentre, 3) = 0.0f;

    for (i = 0; i < elements->m_size; ++i)
    {
        const hkpMassElement *el = &elements->m_data[i];
        hkVector4 worldCentre;
        float mass = el->m_properties.m_mass;
        int lane;

        hkVector4_setTransformedPos(&worldCentre, &el->m_transform, &el->m_properties.m_centerOfMass);
        for (lane = 0; lane < 4; ++lane)
            LANE(weightedCentre, lane) += LANE(worldCentre, lane) * mass;

        totalMass += mass;
        totalVolume += el->m_properties.m_volume;
    }

    if (totalMass <= 0.0f)
        return HK_FAILURE;

    result->m_mass = totalMass;
    result->m_volume = totalVolume;

    {
        float invMass = 1.0f / totalMass;
        int lane;
        for (lane = 0; lane < 4; ++lane)
            LANE(result->m_centerOfMass, lane) = LANE(weightedCentre, lane) * invMass;
    }

    /* Zero the accumulated inertia tensor. */
    for (i = 0; i < 4; ++i)
    {
        LANE(result->m_inertiaTensor.m_col0, i) = 0.0f;
        LANE(result->m_inertiaTensor.m_col1, i) = 0.0f;
        LANE(result->m_inertiaTensor.m_col2, i) = 0.0f;
    }

    for (i = 0; i < elements->m_size; ++i)
    {
        const hkpMassElement *el = &elements->m_data[i];
        hkMatrix3 tensor = el->m_properties.m_inertiaTensor;
        hkVector4 worldCentre;
        float m = el->m_properties.m_mass;
        float rx, ry, rz;

        hkMatrix3_changeBasis(&tensor, &el->m_transform.m_rotation);

        hkVector4_setTransformedPos(&worldCentre, &el->m_transform, &el->m_properties.m_centerOfMass);
        rx = LANE(worldCentre, 0) - LANE(result->m_centerOfMass, 0);
        ry = LANE(worldCentre, 1) - LANE(result->m_centerOfMass, 1);
        rz = LANE(worldCentre, 2) - LANE(result->m_centerOfMass, 2);

        /* Parallel-axis theorem: subtract the off-diagonal products, add the
           diagonal |r|^2-style terms; keep the tensor symmetric. */
        LANE(tensor.m_col0, 1) = LANE(tensor.m_col0, 1) - m * rx * ry; /* Ixy */
        LANE(tensor.m_col1, 0) = LANE(tensor.m_col0, 1);
        LANE(tensor.m_col1, 2) = LANE(tensor.m_col1, 2) - m * rz * ry; /* Iyz */
        LANE(tensor.m_col2, 0) = LANE(tensor.m_col2, 0) - m * rx * rz; /* Ixz */
        LANE(tensor.m_col2, 1) = LANE(tensor.m_col1, 2);
        LANE(tensor.m_col0, 2) = LANE(tensor.m_col2, 0);
        LANE(tensor.m_col2, 2) = LANE(tensor.m_col2, 2) + m * (rx * rx + ry * ry); /* Izz */
        LANE(tensor.m_col0, 0) = LANE(tensor.m_col0, 0) + m * (ry * ry + rz * rz); /* Ixx */
        LANE(tensor.m_col1, 1) = LANE(tensor.m_col1, 1) + m * (rx * rx + rz * rz); /* Iyy */

        hkMatrix3_add(&result->m_inertiaTensor, &tensor);
    }

    return HK_SUCCESS;
}

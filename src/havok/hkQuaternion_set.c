#include "../headers/havok/hkQuaternion.h"
#include "../headers/havok/hkRotation.h"

/* hkQuaternion::set — build the quaternion from a 3x3 rotation matrix. This is a
   thin thunk to the shared matrix->quaternion converter (the DB marks it a
   thunk); &m_vec.m_quad is the leading __vector4 of the hkVector4, i.e. &m_vec. */
extern void quaternionFromRotatation(hkVector4 *quatOut, const hkRotation *r);

void hkQuaternion_set(hkQuaternion *self, const hkRotation *r)
{
    quaternionFromRotatation(&self->m_vec, r);
}

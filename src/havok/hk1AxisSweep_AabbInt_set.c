#include "../headers/havok/hk1AxisSweep.h"
#include "../headers/havok/hkAabb.h"

/* Monotonic IEEE-754-float -> sortable-uint32 remap used to quantise an AABB
   coordinate: for a positive float the sign bit is left clear and the value is
   shifted right by one; for a negative float all bits below the sign are
   inverted. Concretely q = (((int)u >> 31) | 0x80000001) ^ u) >> 1, which yields
   a key whose unsigned ordering matches the real-number ordering. */
static unsigned int hk1AxisSweep_quantize(unsigned int u)
{
    return ((((int)u >> 31) | 0x80000001) ^ u) >> 1;
}

/* hk1AxisSweep::AabbInt::set — quantise a float AABB into the integer sweep
   representation and stash the leaf key. Lanes 0..2 hold the quantised min/max;
   the max is nudged up by one so touching intervals still register as
   overlapping. The leaf key is stored in the 4th min word. */
void hk1AxisSweep_AabbInt_set(hk1AxisSweep_AabbInt *self, const hkAabb *aabbIn, int key)
{
    const unsigned int *mn = aabbIn->m_min.m_quad.___u0.vector4_u32;
    const unsigned int *mx = aabbIn->m_max.m_quad.___u0.vector4_u32;

    self->m_min[0] = hk1AxisSweep_quantize(mn[0]);
    self->m_min[1] = hk1AxisSweep_quantize(mn[1]);
    self->m_min[2] = hk1AxisSweep_quantize(mn[2]);
    self->m_min[3] = (unsigned int)key;
    self->m_max[0] = hk1AxisSweep_quantize(mx[0]) + 1;
    self->m_max[1] = hk1AxisSweep_quantize(mx[1]) + 1;
    self->m_max[2] = hk1AxisSweep_quantize(mx[2]) + 1;
}

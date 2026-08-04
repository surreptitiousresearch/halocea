#include "../headers/havok/hkdGeometry.h"
#include "../headers/havok/hkdGeometry_ObjectIdentifier.h"
#include "../headers/havok/hkMatrix4.h"
#include "../headers/havok/hkBool.h"

/* Boundary callees (2-level descent). */
extern hkBool *hkMatrix4_isApproximatelyEqual(hkBool *result, const hkMatrix4 *a, const hkMatrix4 *b, float tolerance); /* hkMatrix4::isApproximatelyEqual */
extern int hkString_strCmp(const char *a, const char *b);                                                                  /* hkString::strCmp */
extern void hkArrayUtil__reserveMore(void *array, int sizeElem);                                                            /* hkArrayUtil::_reserveMore */

/* hkdGeometry::addSourceObjectIdentifier — return the index of an object
   identifier equal to *oi if the parent geometry already holds one, otherwise
   append a copy and return the index it was stored at. Equality is same inverted
   flag, identical name, and approximately-equal transform. */
int hkdGeometry_addSourceObjectIdentifier(hkdGeometry *self, const hkdGeometry_ObjectIdentifier *oi)
{
    hkBool scratch; /* sret result holder for hkMatrix4_isApproximatelyEqual */
    hkdGeometry *parent = self->m_parent;
    int index = 0;

    if (parent->m_objectIds.m_size > 0)
    {
        int i = 0;
        while (1)
        {
            hkdGeometry_ObjectIdentifier *entry = &parent->m_objectIds.m_data[i];
            if (entry->m_isInverted.m_bool == (oi->m_isInverted.m_bool != 0)
                && !hkString_strCmp(entry->m_name, oi->m_name)
                && hkMatrix4_isApproximatelyEqual(&scratch, &entry->m_transform, &oi->m_transform, 0.001f)->m_bool)
            {
                return (unsigned short)index;
            }
            parent = self->m_parent;
            ++index;
            ++i;
            if (index >= parent->m_objectIds.m_size)
                break;
        }
    }

    /* Append: grow the array if it is at capacity, then copy the 80-byte record. */
    {
        hkdGeometry *owner = self->m_parent;
        hkArray<ObjectIdentifier> *objectIds = &owner->m_objectIds;
        int oldSize = (unsigned short)owner->m_objectIds.m_size;

        if (owner->m_objectIds.m_size == (owner->m_objectIds.m_capacityAndFlags & 0x3FFFFFFF))
            hkArrayUtil__reserveMore(&owner->m_objectIds.m_data, 80);

        {
            int slot = objectIds->m_size;
            objectIds->m_size = slot + 1;
            objectIds->m_data[slot] = *oi;
        }
        return oldSize;
    }
}

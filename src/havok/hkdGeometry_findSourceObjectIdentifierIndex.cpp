/* ?findSourceObjectIdentifierIndex@hkdGeometry@@QBAHABUObjectIdentifier@1@@Z @0x8384FF38 */
#include "../headers/havok/hkdGeometry.h"
#include "../headers/havok/hkdGeometry_ObjectIdentifier.h"
#include "../headers/havok/hkMatrix4.h"
#include "../headers/havok/hkBool.h"

/* Boundary callees (2-level descent). isApproximatelyEqual writes a scratch
   matrix and returns a pointer whose high byte is the boolean result; strCmp is
   the standard Havok string compare (0 == equal). */
extern hkBool *hkMatrix4_isApproximatelyEqual(hkBool *result, const hkMatrix4 *a, const hkMatrix4 *b, float tolerance); /* hkMatrix4::isApproximatelyEqual */
extern int hkString_strCmp(const char *a, const char *b);                                                                  /* hkString::strCmp */

/* hkdGeometry::findSourceObjectIdentifierIndex — linear search of the parent
   geometry's object-identifier array for an entry equal to *id (same inverted
   flag, approximately-equal transform, and identical name). Returns the index or
   -1 if none matches. */
int hkdGeometry_findSourceObjectIdentifierIndex(hkdGeometry *self, const hkdGeometry_ObjectIdentifier *id)
{
    hkBool scratch; /* sret result holder for hkMatrix4_isApproximatelyEqual */
    int count = self->m_parent->m_objectIds.m_size;
    int index = 0;
    int i;

    if (count <= 0)
        return -1;

    for (i = 0; ; ++i)
    {
        hkdGeometry_ObjectIdentifier *entry = &self->m_parent->m_objectIds.m_data[i];
        int matches = 1;

        /* DEVIATION: the decompiler reads the boolean out of the returned
           matrix's first float lane's high byte; it is the m_bool result. */
        if (entry->m_isInverted.m_bool != (id->m_isInverted.m_bool != 0)
            || !hkMatrix4_isApproximatelyEqual(&scratch, &entry->m_transform, &id->m_transform, 0.001f)->m_bool
            || hkString_strCmp(entry->m_name, id->m_name))
        {
            matches = 0;
        }

        if (matches)
            break;

        if (++index >= count)
            return -1;
    }

    return index;
}

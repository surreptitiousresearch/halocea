#include "../headers/havok/hkString.h"
#include "../headers/havok/hkClassMemberTypeProperties.h"

/* Table of per-type reflection properties; indexed by hkClassMemberType.
   Global label ClassMemberProperties @ 0x842167A0, 12-byte rows. */
extern const hkClassMemberTypeProperties ClassMemberProperties[];

/* boundary — Havok SDK, not reversed */
extern void hkString_printf(hkString *self, const char *fmt, ...);          /* hkString::printf(char const *,...) */
extern int hkString_strLen(const char *str);                               /* hkString::strLen(char const *) */
extern void hkString_memCpy(void *dst, const void *src, int n);            /* hkString::memCpy(void *,void const *,int) */
extern void hkArrayUtil__reserve(void *array, int numElem, int sizeElem);   /* hkArrayUtil::_reserve(void *,int,int) */

/* Writes the printable name of a class-member type into ret. With a nonzero
   element count it formats "name[nelem]"; otherwise it copies the bare type name
   (or the empty string, when the type has no name). */
void getSimpleTypeName(hkClassMemberType type, int nelem, hkString *ret)
{
    const char *m_name;
    int nameLength;
    int requiredSize;
    int capacity;
    int newCapacity;
    char *destData;

    if (nelem)
    {
        hkString_printf(ret, "%s[%i]", ClassMemberProperties[type].m_name, nelem);
    }
    else
    {
        m_name = ClassMemberProperties[type].m_name;
        if (m_name && *m_name)
        {
            nameLength = hkString_strLen(ClassMemberProperties[type].m_name);
            requiredSize = nameLength + 1;
            capacity = ret->m_string.m_capacityAndFlags & 0x3FFFFFFF;
            if (capacity < requiredSize)
            {
                newCapacity = 2 * capacity;
                if (requiredSize >= newCapacity)
                    newCapacity = requiredSize;
                hkArrayUtil__reserve(ret, newCapacity, 1);
            }
            ret->m_string.m_size = requiredSize;
            hkString_memCpy(ret->m_string.m_data, m_name, requiredSize);
        }
        else
        {
            if ((ret->m_string.m_capacityAndFlags & 0x3FFFFFFF) == 0)
            {
                int newCapacity = 1;  /* room for the lone NUL terminator */
                hkArrayUtil__reserve(ret, newCapacity, 1);
            }
            destData = ret->m_string.m_data;
            ret->m_string.m_size = 1;
            *destData = 0;
        }
    }
}

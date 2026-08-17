/* ?isSuperClass@hkClass@@QBA?AVhkBool@@ABV1@@Z @0x838DF498 */
#include "../headers/havok/hkClass.h"
#include "../headers/havok/hkBool.h"

extern int hkString_strCmp(const char *a, const char *b); /* hkString::strCmp */

/* hkClass::isSuperClass — returns (via the sret hkBool) whether `self` is `other`
   or one of its ancestor classes, comparing by class name. Walks `other`'s
   parent chain looking for a name equal to `self`'s.
   The sret hkBool is returned; its byte is written through the m_bool field. */
hkBool *hkClass_isSuperClass(hkBool *out, const hkClass *self, const hkClass *other)
{
    const hkClass *cls = other;
    if (cls)
    {
        while (hkString_strCmp(cls->m_name, self->m_name))
        {
            cls = cls->m_parent;
            if (!cls)
            {
                out->m_bool = 0;
                return out;
            }
        }
        out->m_bool = 1;
        return out;
    }
    out->m_bool = 0;
    return out;
}

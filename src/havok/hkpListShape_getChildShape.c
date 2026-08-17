/* ?getChildShape@hkpListShape@@UBAPBVhkpShape@@IAAY0CAA@D@Z @0x83605E20 */
#include "../headers/havok/hkpListShape.h"

/* hkpListShape::getChildShape — fetch the child hkpShape* for a shape key.

   Ground truth (disasm 0x83605E30):
       lwz r11, 8(r3)     ; base = *(void**)(this+8)  (hkpShape::m_userData,
                          ;   repurposed to cache the ChildInfo array base)
       slwi r10, r4, 4    ; key * 16   (sizeof ChildInfo)
       add  r11, r10, r11
       lwz  r3, 4(r11)    ; return *(hkpShape**)(base + 16*key + 4)
   DEVIATION: the decompiler rendered this as *(hkpShape**)(16*key + m_userData),
   dropping the +4; the disassembly's explicit lwz r3,4(r11) is authoritative.
   The output buffer argument is unused on this path. */
const hkpShape *hkpListShape_getChildShape(hkpListShape *this, unsigned int key, char (*buffer)[512])
{
    const char *childBase = (const char *)(size_t)this->base.base.m_userData;
    (void)buffer;
    return *(const hkpShape *const *)(childBase + 16 * key + 4);
}

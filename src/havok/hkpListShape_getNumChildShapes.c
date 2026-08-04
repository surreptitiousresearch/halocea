#include "../headers/havok/hkpListShape.h"

/* hkpListShape::getNumChildShapes — total child-shape count.

   Ground truth (disasm 0x83606A20):
       lhz  r11, 0x16(r3)   ; *(u16)(this+22)
       lwz  r10, 0x0C(r3)   ; *(u32)(this+12)
       subf r3, r11, r10    ; r10 - r11
   hkpListShape overloads two base-class slots to cache these counts: the total
   is stored in hkpShape::m_type's storage word (offset 12) and the disabled
   count in the two bytes just past hkpShapeCollection::m_collectionType
   (offset 22). Reproduced via those base fields. */
unsigned int hkpListShape_getNumChildShapes(hkpListShape *this)
{
    unsigned int total = this->base.base.m_type; /* offset 12 */
    unsigned short disabled =
        *(const unsigned short *)((const char *)&this->base.m_collectionType + 1); /* offset 22 */
    return total - disabled;
}

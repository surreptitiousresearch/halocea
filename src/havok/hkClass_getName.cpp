/* ?getName@hkClass@@QBAPBDXZ @0x838DF458 */
#include "../headers/havok/hkClass.h"

/* hkClass::getName — accessor for the reflection descriptor's name. */
const char *hkClass_getName(const hkClass *self)
{
    return self->m_name;
}

#include "../headers/havok/hkdProperties.h"

/* hkpProperty::mapStringToKey @ 0x83911518
   Hash a NUL-terminated property name into a 31-bit key (h = h*31 + c). */
void hkpProperty_mapStringToKey(const char *name, unsigned int *keyOut)
{
    int hash = 0;
    const char *p;
    for (p = name; *p; ++p)
        hash = 31 * hash + *p;
    *keyOut = (unsigned int)(hash & 0x7FFFFFFF);
}

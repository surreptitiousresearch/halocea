/* hkIsDigit @0x838EE120 */
#include "../headers/havok/hkBool.h"

/* result = (c is an ASCII decimal digit '0'..'9'). */
hkBool *hkIsDigit(hkBool *result, int c)
{
    result->m_bool = (c >= '0' && c <= '9');
    return result;
}

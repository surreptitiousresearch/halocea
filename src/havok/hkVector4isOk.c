/* hkVector4isOk @0x838D7A20 */
#include "../headers/havok/hkBool.h"

/* result = (all n floats are finite) — i.e. none has an all-ones exponent
   (inf/NaN). An empty range is trivially ok. */
hkBool *hkVector4isOk(hkBool *result, const float *f, int n)
{
    int i = 0;

    if (n <= 0)
    {
        result->m_bool = 1;
        return result;
    }

    while ((*(const unsigned int *)f & 0x7F800000) != 0x7F800000)
    {
        ++i;
        ++f;
        if (i >= n)
        {
            result->m_bool = 1;
            return result;
        }
    }
    result->m_bool = 0;
    return result;
}

#include "../headers/havok/hkMatrix4.h"
#include "../headers/havok/hkBool.h"

/* hkMatrix4::isOk — report whether every one of the matrix's 16 floats is finite
   (no all-ones exponent => no inf/NaN). The result byte is written into the
   caller-supplied scratch buffer (top byte of its first word on this big-endian
   target, i.e. its first byte) and that buffer is returned as the hkBool.
   The matrix is scanned as 16 contiguous 32-bit words, matching the DB body. */
hkBool *hkMatrix4_isOk(hkMatrix4 *scratch, const hkMatrix4 *m)
{
    const unsigned int *word = (const unsigned int *)m;
    int i = 0;

    while ((word[i] & 0x7F800000) != 0x7F800000)
    {
        if (++i >= 16)
        {
            ((hkBool *)scratch)->m_bool = 1;
            return (hkBool *)scratch;
        }
    }
    ((hkBool *)scratch)->m_bool = 0;
    return (hkBool *)scratch;
}

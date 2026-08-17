/* hkCalcQuantizedSize @0x83FA1888 */
#include "../headers/havok/hkaQuantizeDesc.h"

/* hkCalcQuantizedSize — byte size of the quantized stream for nValues samples.
   The non-preserved values (nValues - m_preserved) each take m_bitWidth bits,
   rounded up to whole bytes; the preserved values are stored as full 32-bit
   floats, contributing m_preserved*4 bytes.
   DEVIATION: the original expresses the preserved-byte term as __ROL4__(preserved,2)
   (rotate-left by 2); reproduced as the equivalent 32-bit rotate. */
int hkCalcQuantizedSize(unsigned int nValues, const hkaQuantizeDesc *desc)
{
    unsigned int preserved = desc->m_preserved;
    unsigned int packedBits = (nValues - preserved) * desc->m_bitWidth + 7;
    unsigned int preservedBytes = (preserved << 2) | (preserved >> 30); /* __ROL4__(preserved, 2) */
    return (int)(packedBits / 8) + (int)preservedBytes;
}

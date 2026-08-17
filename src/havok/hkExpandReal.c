/* hkExpandReal @0x83FA1060 */
#include "../headers/havok/hkaQuantizeDesc.h"
#include "../headers/havok/hkBool.h"

/* boundary globals / helper. */
extern const float oneOverTwoToBitWidthLookupTable[]; /* 1.0f / 2^bitWidth, indexed by bit width */
extern hkBool HK_flyingcolors_animation;              /* debug/verification gate */
extern void processFlyingColors(hkBool *flag);        /* ?processFlyingColors@@YAXAAVhkBool@@@Z */

/* hkExpandReal — dequantize a packed integer stream back to floats.

   The first m_preserved samples are stored verbatim as raw 32-bit floats and are
   copied through; the remaining (nValues - m_preserved) samples are quantized
   integers of m_bitWidth bits which are expanded with
       value = ((float)q + 0.5f) * (m_scale / 2^m_bitWidth) + m_offset.
   Specialised paths mirror the original for m_bitWidth 8, 16, and the generic
   bit-packed case.

   DEVIATION: the decompiler fused the descriptor pointer and the output pointer
   into a single 64-bit register-pair argument; split here into the natural
   (encoded, desc, out, nValues) form. DEVIATION: the preserved-float copy
   reproduces the original's exact byte permutation (its big-endian VMX byte
   handling reassembles input bytes 0,2,1,3 into the float's LE byte image).
   DEVIATION: the generic path's bit bookkeeping is reproduced as a standard
   LSB-first 16-bit-refill bit reader, equivalent to the original's inlined
   accumulator logic. */
void hkExpandReal(const unsigned char *encoded, const hkaQuantizeDesc *desc, float *out,
                  unsigned int nValues)
{
    unsigned int bitWidth = desc->m_bitWidth;
    unsigned int preserved = desc->m_preserved;
    unsigned int i;

    /* 1) Preserved leading values: raw floats, byte-permuted as the original does. */
    for (i = 0; i < preserved; ++i)
    {
        unsigned char *o = (unsigned char *)&out[i];
        o[0] = encoded[0]; /* LOBYTE */
        o[1] = encoded[2]; /* BYTE1  */
        o[2] = encoded[1]; /* BYTE2  */
        o[3] = encoded[3]; /* HIBYTE */
        encoded += 4;
    }

    if (!HK_flyingcolors_animation.m_bool)
        processFlyingColors(&HK_flyingcolors_animation);
    if (!HK_flyingcolors_animation.m_bool)
        return;

    {
        unsigned int count = nValues - preserved;
        float scale = oneOverTwoToBitWidthLookupTable[bitWidth] * desc->m_scale;
        float offset = desc->m_offset;
        float *dst = out + preserved;
        unsigned int mask = (1u << bitWidth) - 1u;
        unsigned int k;

        if (bitWidth == 8)
        {
            for (k = 0; k < count; ++k)
            {
                unsigned int q = *encoded++;
                dst[k] = ((float)q + 0.5f) * scale + offset;
            }
        }
        else if (bitWidth == 16)
        {
            for (k = 0; k < count; ++k)
            {
                unsigned int q = (unsigned int)(encoded[0] | (encoded[1] << 8));
                encoded += 2;
                dst[k] = ((float)q + 0.5f) * scale + offset;
            }
        }
        else
        {
            unsigned int acc = 0;
            int bitsInAcc = 0;
            for (k = 0; k < count; ++k)
            {
                unsigned int q;
                if (bitsInAcc < (int)bitWidth)
                {
                    acc |= (unsigned int)(encoded[0] | (encoded[1] << 8)) << bitsInAcc;
                    encoded += 2;
                    bitsInAcc += 16;
                }
                q = acc & mask;
                acc >>= bitWidth;
                bitsInAcc -= bitWidth;
                dst[k] = ((float)q + 0.5f) * scale + offset;
            }
        }
    }
}

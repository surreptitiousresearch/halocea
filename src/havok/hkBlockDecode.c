/* hkBlockDecode @0x83FA1520 */
#include "../headers/havok/hkaBlockDesc.h"

/* hkBlockDecode — decode one run-length + bit-packed block back into a raw
   quantized byte/short stream.

   Wire format (per block):
     - the first m_preserved values are stored verbatim as 32-bit floats and are
       copied straight through to the output;
     - a run/literal flag bitstream (one bit per symbol, LSB-first within each
       byte) selects, for each of the numSymbols symbols, either the run symbol
       (m_runSymbol) or a literal read of m_bitWidth bits from the packed data
       that follows the flag bytes.
   Three specialised paths mirror the original: m_bitWidth==8 (byte output),
   m_bitWidth==16 (16-bit output), and a generic bit-packing path.
   Returns the number of bytes consumed from the block.

   DEVIATION: the original's __twllei (trap-if bitWidth<=0) divide guard is
   dropped. DEVIATION: the generic path's output cursor / bit-account advance
   the same symbol width in both branches; the decompiler rendered a couple of
   these as HIBYTE(m_bitWidth), a register misread — reproduced as m_bitWidth. */
/* DEVIATION: rawData is `unsigned char *`, not `float *` — the DB prototype says
   `unsigned __int8 *rawData` and the body agrees, casting the pointer to bytes on its very
   next use and staying in bytes for all three width paths. Only the preserved prefix is
   copied as 32-bit units. */
unsigned int hkBlockDecode(const unsigned char *encodedData, hkaBlockDesc *desc,
                           unsigned char *rawData, unsigned int rawDataSizeBytes)
{
    unsigned int preserved = desc->m_preserved;
    unsigned int bitWidth = desc->m_bitWidth;
    unsigned int mask = (1u << bitWidth) - 1u;

    /* Number of RLE symbols encoded in this block. */
    unsigned int numSymbols = 8u * (rawDataSizeBytes - 4u * preserved) / bitWidth;

    unsigned int i;
    unsigned int preservedBytes = 4u * preserved;

    /* Copy the preserved leading floats verbatim. */
    for (i = 0; i < preserved; ++i)
        ((float *)rawData)[i] = ((const float *)encodedData)[i];

    {
        const unsigned char *flagPtr = encodedData + preservedBytes;
        unsigned char *out = rawData + preservedBytes;
        unsigned int flagByteCount = (numSymbols + 7u) >> 3;
        const unsigned char *packed = &encodedData[preservedBytes + 2 + flagByteCount];

        /* 16-bit little-endian bit accumulator seeded from the first two packed bytes. */
        unsigned int accumulator =
            (unsigned int)encodedData[preservedBytes + flagByteCount]
            | ((unsigned int)encodedData[preservedBytes + 1 + flagByteCount] << 8);
        int bitsAvailable = 16;
        unsigned char flagMask = 1;
        unsigned int decodedCount = 0;

        if (bitWidth == 8)
        {
            for (i = 0; i < numSymbols; ++i)
            {
                if ((*flagPtr & flagMask) != 0)
                {
                    *out++ = (unsigned char)desc->m_runSymbol;
                }
                else
                {
                    if (bitsAvailable < 8)
                    {
                        accumulator |= (unsigned int)(packed[0] | (packed[1] << 8)) << bitsAvailable;
                        packed += 2;
                        bitsAvailable += 16;
                    }
                    *out++ = (unsigned char)(mask & accumulator);
                    accumulator >>= 8;
                    bitsAvailable -= 8;
                    ++decodedCount;
                }
                flagMask = (unsigned char)((flagMask << 1) & ~0x1u); /* rolling bit cursor; &~1 = decompiler byte-truncation artifact */
                if (flagMask == 0) { flagMask = 1; ++flagPtr; }
            }
        }
        else if (bitWidth == 16)
        {
            unsigned char runLo = (unsigned char)desc->m_runSymbol;
            unsigned char runHi = (unsigned char)(desc->m_runSymbol >> 16); /* BYTE2 */
            for (i = 0; i < numSymbols; ++i)
            {
                if ((*flagPtr & flagMask) != 0)
                {
                    out[0] = runLo;
                    out[1] = runHi;
                }
                else
                {
                    if (bitsAvailable < 16)
                    {
                        accumulator |= (unsigned int)(packed[0] | (packed[1] << 8)) << bitsAvailable;
                        packed += 2;
                        bitsAvailable += 16;
                    }
                    out[0] = (unsigned char)accumulator;
                    out[1] = (unsigned char)(accumulator >> 16); /* BYTE2 */
                    accumulator >>= 16;
                    bitsAvailable -= 16;
                    ++decodedCount;
                }
                out += 2;
                flagMask = (unsigned char)((flagMask << 1) & ~0x1u); /* rolling bit cursor; &~1 = decompiler byte-truncation artifact */
                if (flagMask == 0) { flagMask = 1; ++flagPtr; }
            }
        }
        else
        {
            unsigned int outAcc = 0;       /* packed output accumulator */
            unsigned char outBits = 0;     /* bits currently buffered in outAcc */
            for (i = 0; i < numSymbols; ++i)
            {
                unsigned char prevMask = flagMask;
                if ((*flagPtr & flagMask) != 0)
                {
                    outAcc |= desc->m_runSymbol << outBits;
                    outBits = (unsigned char)(outBits + bitWidth);
                    if (outBits > 15)
                    {
                        out[0] = (unsigned char)outAcc;
                        out[1] = (unsigned char)(outAcc >> 16);
                        outAcc >>= 16;
                        outBits = (unsigned char)(outBits - 16);
                        out += 2;
                    }
                }
                else
                {
                    if (bitsAvailable < (int)bitWidth)
                    {
                        accumulator |= (unsigned int)(packed[0] | (packed[1] << 8)) << bitsAvailable;
                        packed += 2;
                        bitsAvailable += 16;
                    }
                    outAcc |= (accumulator & mask) << outBits;
                    outBits = (unsigned char)(outBits + bitWidth);
                    if (outBits > 15)
                    {
                        out[0] = (unsigned char)outAcc;
                        out[1] = (unsigned char)(outAcc >> 16);
                        outAcc >>= 16;
                        outBits = (unsigned char)(outBits - 16);
                        out += 2;
                    }
                    ++decodedCount;
                    accumulator >>= bitWidth;
                    bitsAvailable -= bitWidth;
                }
                flagMask = (unsigned char)((2 * prevMask) & ~0x1u); /* rolling bit cursor; &~1 = decompiler byte-truncation artifact */
                if (flagMask == 0) { flagMask = 1; ++flagPtr; }
            }
            if (outBits != 0)
            {
                out[0] = (unsigned char)outAcc;
                if (outBits > 7)
                    out[1] = (unsigned char)(outAcc >> 16);
            }
        }

        return (bitWidth * decodedCount + 32u * preserved + numSymbols + 7u) >> 3;
    }
}

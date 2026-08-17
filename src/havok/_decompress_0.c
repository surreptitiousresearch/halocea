/* _decompress_0 @0x838C53F8 */
#include "../headers/havok/hkDeltaDecompressionParameters.h"
#include "../headers/havok/hkaQuantizeDesc.h"

/*
 * hkaDeltaCompressedAnimation -- delta-compressed animation block decoder
 * (file-static _decompress, IDA symbol _decompress_0). Reconstructed from a
 * badly ABI-modelled decompile; the disassembly (0x838C53F8) is authoritative.
 *
 * CAVEAT (deviations from the decompiler output):
 *  - TRUE SIGNATURE. The decompiler guessed `_decompress_0(unsigned int, float*,
 *    float*, int, float*)` with a phantom r2 read. Disasm shows a 32-byte
 *    hkDeltaDecompressionParameters passed BY VALUE across r3..r6
 *    (r3 = {m_numD,m_blockSize}, r4 = {m_numValues,m_preserved,m_maxBitWidth,pad},
 *    r5 = {m_offset,m_scale}, r6 = {m_bitWidth,m_quantizedData}) and the output
 *    float* in r7 (r31).
 *  - Unlike the wavelet path there is no RLE hkBlockDecode and no per-thread
 *    scratch buffer: quantized values are read straight from m_quantizedData at a
 *    running byte offset advanced by hkCalcQuantizedSize each track.
 *  - The value count handed to the codec helpers is m_numValues (r28 =
 *    arg_18+0); the output pointer advances by m_blockSize floats per track
 *    (disasm: `slwi r25, m_blockSize, 2`). These are distinct fields, kept so.
 *  - offset[i] is read from scale[i] via a fixed pointer delta captured once
 *    (disasm: `subf r22, m_scale, m_offset` outside the loop; `lfsx` inside).
 *  - hkExpandReal / hkInverseDeltaTransform / hkCalcQuantizedSize are Havok
 *    boundary helpers (bounded descent: kept extern).
 */

/* Boundaries -- Havok delta-codec helpers, not re-sourced here. */
extern int hkCalcQuantizedSize(int nValues, hkaQuantizeDesc *desc);
extern void hkExpandReal(const unsigned char *rawData, hkaQuantizeDesc *desc,
                         float *out, int nValues);
extern void hkInverseDeltaTransform(float *data, int nValues);

/* Decode one delta-compressed animation chunk: for each of m_numD tracks, expand
   the quantized values to floats and run the inverse delta transform in place
   into `out` (m_blockSize floats of stride per track). */
/* file-static in the original TU; external here because the corpus splits caller
   (decompressBlockToCache_0.cpp) and callee into separate TUs — IDA symbol _decompress_0 */
void _decompress_0(hkDeltaDecompressionParameters params, float *out)
{
    hkaQuantizeDesc desc;
    desc.m_preserved = params.m_preserved;

    if (params.m_numD)
    {
        const float *scalePtr = params.m_scale;
        /* offset[i] == *(scalePtr + this byte delta); delta is constant. */
        long offsetByteDelta = (const char *)params.m_offset - (const char *)params.m_scale;
        int quantizedOffset = 0;
        unsigned int trackIndex = 0;
        do
        {
            desc.m_scale = *scalePtr;
            desc.m_offset = *(const float *)((const char *)scalePtr + offsetByteDelta);
            desc.m_bitWidth = params.m_bitWidth ? params.m_bitWidth[trackIndex]
                                                : params.m_maxBitWidth;

            hkExpandReal(params.m_quantizedData + quantizedOffset, &desc, out, params.m_numValues);
            hkInverseDeltaTransform(out, params.m_numValues);

            out += params.m_blockSize; /* stride = m_blockSize floats per track */
            ++trackIndex;
            quantizedOffset += hkCalcQuantizedSize(params.m_numValues, &desc);
            ++scalePtr;
        } while (trackIndex < params.m_numD);
    }
}

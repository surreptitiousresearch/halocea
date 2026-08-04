#include "../headers/havok/hkWaveletDecompressionParameters.h"
#include "../headers/havok/hkaQuantizeDesc.h"
#include "../headers/havok/hkaBlockDesc.h"
#include "../headers/havok/hkThreadMemory.h"

/*
 * hkaWaveletCompressedAnimation -- wavelet-compressed animation block decoder
 * (file-static _decompress). Reconstructed from a badly ABI-modelled decompile;
 * the disassembly (0x838BE290) is authoritative.
 *
 * CAVEAT (deviations from the decompiler output):
 *  - TRUE SIGNATURE. The decompiler guessed `decompress(unsigned int, __int64,
 *    int, float*)` with a phantom r2 read. Disasm shows a 28-byte
 *    hkWaveletDecompressionParameters passed BY VALUE, split across r3..r6
 *    (r3 = {m_numD,m_blockSize}, r4 = {m_preserved,m_maxBitWidth,pad,m_offset},
 *    r5 = {m_scale,m_bitWidth}, r6 = {m_encodedData,pad}), and the output float*
 *    in r7. The caller
 *    (hkaWaveletCompressedAnimation::samplePartialWithDataChunks @0x838BE450)
 *    stages exactly these four doublewords (stw pairs -> ld r3/r4/r5, sldi r6)
 *    before `bl _decompress`, confirming the struct-by-value form.
 *  - The scratch "raw values" buffer is bump-allocated from the per-thread LIFO
 *    stack: prologue checks m_current+size vs m_end at hkThreadMemory+0x20/0x2C
 *    (else the vtable slow path), epilogue restores m_current and, if it hit the
 *    base marker (+0x28), calls the vtable notify. Modelled as the established
 *    hkThreadMemory_allocateStack / deallocateStack API, with the TLS deref
 *    *(hkThreadMemory**)(*(int*)r13 + 704) wrapped as hkThreadMemory_getCurrent().
 *  - offset[i] is read from scale[i] via a fixed pointer delta captured once
 *    (disasm: `subf r21, m_scale, m_offset` outside the loop; `lfsx` inside).
 *  - The quantized "run symbol" is computed as
 *    (long long)((-offset/scale) * (1<<bitWidth)); disasm truncates via fctidz
 *    and keeps the low word.
 *  - hkCalcQuantizedSize / hkBlockDecode / hkExpandReal / hkInverseWaveletTransform
 *    are Havok boundary helpers (bounded descent: kept extern).
 */

/* Boundaries -- Havok wavelet-codec helpers, not re-sourced here. */
extern int hkCalcQuantizedSize(int nValues, hkaQuantizeDesc *desc);
/* DB prototype: unsigned int hkBlockDecode(const unsigned __int8 *encodedData, hkaBlockDesc *desc,
   unsigned __int8 *rawData, unsigned int rawDataSizeBytes) — it decodes into the *quantized byte*
   buffer that hkExpandReal then widens to float, so the third parameter is not a float *. */
extern unsigned int hkBlockDecode(const unsigned char *encodedData, hkaBlockDesc *desc,
                                  unsigned char *rawData, unsigned int rawDataSizeBytes);
extern void hkExpandReal(const unsigned char *rawData, hkaQuantizeDesc *desc,
                         float *out, int nValues);
extern void hkInverseWaveletTransform(float *data, int nValues);

/* Decode one wavelet-compressed animation chunk: for each of m_numD tracks,
   RLE-decode the quantized block, expand it to floats, and run the inverse
   wavelet transform in place into `out` (m_blockSize floats per track). */
static void decompress(hkWaveletDecompressionParameters params, float *out)
{
    /* Worst-case scratch for one block's raw quantized values (max bit width),
       rounded up to a 16-byte multiple, taken from the per-thread stack. */
    hkaQuantizeDesc desc;
    desc.m_preserved = params.m_preserved;
    desc.m_bitWidth = params.m_maxBitWidth;

    int rawByteSize = hkCalcQuantizedSize(params.m_blockSize, &desc);
    int scratchSize = (rawByteSize + 16) & 0xFFFFFFF0;
    unsigned char *rawData =
        (unsigned char *)hkThreadMemory_allocateStack(hkThreadMemory_getCurrent(), scratchSize);

    const unsigned char *encoded = params.m_encodedData;

    if (params.m_numD)
    {
        const float *scalePtr = params.m_scale;
        /* offset[i] == *(scalePtr + this byte delta); delta is constant. */
        long offsetByteDelta = (const char *)params.m_offset - (const char *)params.m_scale;
        unsigned int trackIndex = 0;
        do
        {
            hkaBlockDesc blockDesc;

            desc.m_scale = *scalePtr;
            desc.m_offset = *(const float *)((const char *)scalePtr + offsetByteDelta);
            desc.m_bitWidth = params.m_bitWidth ? params.m_bitWidth[trackIndex]
                                                : params.m_maxBitWidth;

            int trackRawBytes = hkCalcQuantizedSize(params.m_blockSize, &desc);

            int quantLevels = 1 << desc.m_bitWidth;
            blockDesc.m_bitWidth = desc.m_bitWidth;
            /* Quantized index of value 0.0 -- the run/no-change symbol. */
            unsigned int runSymbol =
                (unsigned int)(long long)(((-desc.m_offset) / desc.m_scale) * (float)quantLevels);
            blockDesc.m_runSymbol = runSymbol;
            if (runSymbol == (unsigned int)quantLevels)
                blockDesc.m_runSymbol = runSymbol - 1;

            encoded += hkBlockDecode(encoded, &blockDesc, rawData, trackRawBytes);
            hkExpandReal(rawData, &desc, out, params.m_blockSize);
            hkInverseWaveletTransform(out, params.m_blockSize);

            ++trackIndex;
            out += params.m_blockSize;
            ++scalePtr;
        } while (trackIndex < params.m_numD);
    }

    hkThreadMemory_deallocateStack(hkThreadMemory_getCurrent(), rawData);
}

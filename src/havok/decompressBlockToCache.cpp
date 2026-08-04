/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
#include "../headers/havok/hkaWaveletCompressedAnimation.h"
#include "../headers/havok/hkWaveletDecompressionParameters.h"
#include "../headers/havok/hkMonitorStream.h"

/* decompressBlockToCache @0x838BEAD8
   Resolves the wavelet-compressed source data for the block containing `pose`,
   packs it into an hkWaveletDecompressionParameters descriptor, and hands it to
   the wavelet decompressor which writes the reconstructed DOFs into `outputCache`.
   The body is bracketed by Havok profiler timer commands (HK_TIMER_BEGIN /
   HK_TIMER_END) appended to the calling thread's monitor stream. */

/* r13-relative thread anchor: *(void**)(r13+0) yields the current thread's Havok
   context, whose monitor-stream pointer sits at byte offset 712. No DB type
   covers this thread block, so the fetch is kept as a raw offset load. */
extern char *hk_currentThreadContext;

/* PPC mftb — reads the low 32 bits of the time-base register. */
extern unsigned int __mftb(void);

/* Wavelet block decompressor @0x838BE290. Takes the 28-byte parameter block by
   value (r3-r6) plus the output cache in r7. Boundary: Havok SDK, not reversed. */
extern void decompress(hkWaveletDecompressionParameters params, float *outputCache);

void decompressBlockToCache(
        const hkaWaveletCompressedAnimation *animation,
        unsigned int pose,
        float *outputCache,
        unsigned int numD)
{
    char *threadContext = hk_currentThreadContext;

    /* HK_TIMER_BEGIN("decompressBlockCacheW") */
    /* Havok thread-context TLS block layout not in DB (re-source halted): monitor-stream
       pointer at +712 kept raw. */
    hkMonitorStream *monitorStream = *(hkMonitorStream **)(threadContext + 712);
    hkMonitorStream_TimerCommand *beginCommand =
        (hkMonitorStream_TimerCommand *)monitorStream->m_end.m_storage;
    if ((unsigned int)beginCommand < (unsigned int)monitorStream->m_capacityMinus16.m_storage)
    {
        beginCommand->m_commandAndMonitor = "TtdecompressBlockCacheW";
        beginCommand->m_time0 = __mftb();
        monitorStream->m_end.m_storage = (char *)beginCommand + sizeof(hkMonitorStream_TimerCommand);
    }

    {
        int blockSize = animation->m_blockSize;
        unsigned char *dataBuffer = animation->m_dataBuffer;
        unsigned int blockIndexTableOffset = animation->m_blockIndexIdx;
        unsigned int quantizedDataOffset = animation->m_quantizedDataIdx;

        /* blockSize is trap-checked non-zero (twllei) before this unsigned divide. */
        unsigned int blockNumber = pose / (unsigned int)blockSize;

        const float *offsetTable =
            (const float *)(dataBuffer + animation->m_qFormat.m_offsetIdx);
        const float *scaleTable =
            (const float *)(dataBuffer + animation->m_qFormat.m_scaleIdx);
        const unsigned char *bitWidthTable =
            dataBuffer + animation->m_qFormat.m_bitWidthIdx;

        unsigned int blockDataOffset =
            *(unsigned int *)(dataBuffer + blockIndexTableOffset + blockNumber * 4);
        const unsigned char *encodedData =
            dataBuffer + (blockDataOffset + quantizedDataOffset);

        hkWaveletDecompressionParameters params;
        params.m_numD = numD;
        params.m_blockSize = blockSize;
        params.m_preserved = animation->m_qFormat.m_preserved;
        params.m_maxBitWidth = animation->m_qFormat.m_maxBitWidth;
        params.m_offset = offsetTable;
        params.m_scale = scaleTable;
        params.m_bitWidth = bitWidthTable;
        params.m_encodedData = encodedData;

        decompress(params, outputCache);
    }

    /* HK_TIMER_END() */
    {
        hkMonitorStream *monitorStreamEnd = *(hkMonitorStream **)(threadContext + 712); /* same DB-absent TLS layout */
        hkMonitorStream_TimerCommand *endCommand =
            (hkMonitorStream_TimerCommand *)monitorStreamEnd->m_end.m_storage;
        if ((unsigned int)endCommand < (unsigned int)monitorStreamEnd->m_capacityMinus16.m_storage)
        {
            endCommand->m_commandAndMonitor = "Et";
            endCommand->m_time0 = __mftb();
            monitorStreamEnd->m_end.m_storage = (char *)endCommand + sizeof(hkMonitorStream_TimerCommand);
        }
    }
}

/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
#include "../headers/havok/hkaDeltaCompressedAnimation.h"
#include "../headers/havok/hkDeltaDecompressionParameters.h"
#include "../headers/havok/hkMonitorStream.h"

/* decompressBlockToCache_0 @0x838C5C60
   Delta-compressed sibling of decompressBlockToCache (the wavelet variant).
   Resolves the delta-quantized source data for the block containing `pose`,
   computes how many poses that block actually reconstructs (clamped against the
   animation's pose count), packs everything into an hkDeltaDecompressionParameters
   descriptor, and hands it to the delta block decompressor (_decompress_0) which
   writes the reconstructed DOFs into `data`. The body is bracketed by Havok
   profiler timer commands (HK_TIMER_BEGIN / HK_TIMER_END) appended to the calling
   thread's monitor stream. */

/* r13-relative thread anchor: *(void**)(r13+0) yields the current thread's Havok
   context, whose monitor-stream pointer sits at byte offset 712. No DB type
   covers this thread block, so the fetch is kept as a raw offset load. */
extern char *hk_currentThreadContext;

/* PPC mftb — reads the low 32 bits of the time-base register. */
extern unsigned int __mftb(void);

/* Delta block decompressor @0x838C53F8 (DB: _decompress_0). Takes the 32-byte parameter block by
   value (r3-r6) plus the output cache in r7. Boundary: Havok SDK, not reversed. */
extern void _decompress_0(hkDeltaDecompressionParameters params, float *data);

void decompressBlockToCache_0(
        const hkaDeltaCompressedAnimation *animation,
        unsigned int pose,
        float *data,
        unsigned int numD)
{
    char *threadContext = hk_currentThreadContext;

    /* HK_TIMER_BEGIN("decompressBlockCacheD") */
    hkMonitorStream *monitorStream = *(hkMonitorStream **)(threadContext + 712);
    hkMonitorStream_TimerCommand *beginCommand =
        (hkMonitorStream_TimerCommand *)monitorStream->m_end.m_storage;
    if ((unsigned int)beginCommand < (unsigned int)monitorStream->m_capacityMinus16.m_storage)
    {
        beginCommand->m_commandAndMonitor = "TtdecompressBlockCacheD";
        beginCommand->m_time0 = __mftb();
        monitorStream->m_end.m_storage = (char *)beginCommand + sizeof(hkMonitorStream_TimerCommand);
    }

    {
        unsigned int blockSize = (unsigned int)animation->m_blockSize;
        int numberOfPoses = animation->m_numberOfPoses;
        unsigned char *dataBuffer = animation->m_dataBuffer;

        /* blockSize is trap-checked non-zero (twllei) before this unsigned divide. */
        unsigned int blockNumber = pose / blockSize;

        /* Number of poses this block reconstructs, clamped so it neither exceeds a
           single-block animation nor runs past the last pose. */
        int posesInBlock = (int)blockSize;
        if ((int)blockSize > numberOfPoses)
        {
            posesInBlock = numberOfPoses;
            blockNumber = 0;
        }
        {
            unsigned int blockStartPose = blockSize * blockNumber;
            if ((int)(blockStartPose + posesInBlock) > numberOfPoses)
                posesInBlock = numberOfPoses - blockStartPose;

            hkDeltaDecompressionParameters params;
            params.m_numD = numD;
            params.m_blockSize = (int)blockSize;
            params.m_numValues = posesInBlock;
            params.m_preserved = animation->m_qFormat.m_preserved;
            params.m_maxBitWidth = animation->m_qFormat.m_maxBitWidth;
            params.m_offset =
                (const float *)(dataBuffer + animation->m_qFormat.m_offsetIdx);
            params.m_scale =
                (const float *)(dataBuffer + animation->m_qFormat.m_scaleIdx);
            params.m_bitWidth = dataBuffer + animation->m_qFormat.m_bitWidthIdx;
            params.m_quantizedData =
                dataBuffer + animation->m_quantizedDataIdx
                + animation->m_totalBlockSize * blockNumber;

            _decompress_0(params, data);
        }
    }

    /* HK_TIMER_END() */
    {
        hkMonitorStream *monitorStreamEnd = *(hkMonitorStream **)(threadContext + 712);
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

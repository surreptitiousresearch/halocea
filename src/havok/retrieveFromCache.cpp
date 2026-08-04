/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
#include "../headers/havok/hkaWaveletCompressedAnimation.h"
#include "../headers/havok/hkaChunkCache.h"
#include "../headers/havok/hkMonitorStream.h"
#include "../headers/havok/hkBool.h"

/* retrieveFromCache @0x838BEBD8
   Returns the decompressed DOF block for pose `poseIdx` of a wavelet-compressed
   animation, using an hkaChunkCache keyed by the animation's full cache key.
   On a hit the cached chunk is returned under a read lock; on a miss a chunk is
   allocated, the block is decompressed into it via decompressBlockToCache, and
   the fresh chunk is returned. All cache access is bracketed by the cache's
   critical section, and the whole body sits inside a Havok profiler timer
   command pair (HK_TIMER_BEGIN("cacheW") / HK_TIMER_END). */

/* r13-relative thread anchor: *(void**)(r13+0) yields the current thread's Havok
   context, whose monitor-stream pointer sits at byte offset 712. No DB type
   covers this thread block, so the fetch is kept as a raw offset load. */
extern char *hk_currentThreadContext;

/* PPC mftb — reads the low 32 bits of the time-base register. */
extern unsigned int __mftb(void);

/* Sibling wavelet block decompressor @0x838BEAD8 — already reversed. */
extern void decompressBlockToCache(
        const hkaWaveletCompressedAnimation *animation,
        unsigned int pose,
        float *outputCache,
        unsigned int numD);

float *retrieveFromCache(
        hkaWaveletCompressedAnimation *animation,
        unsigned int poseIdx,
        unsigned int numD,
        hkaChunkCache **cache)
{
    char *threadContext = hk_currentThreadContext;

    /* HK_TIMER_BEGIN("cacheW") */
    {
        hkMonitorStream *monitorStream = *(hkMonitorStream **)(threadContext + 712);
        hkMonitorStream_TimerCommand *beginCommand =
            (hkMonitorStream_TimerCommand *)monitorStream->m_end.m_storage;
        if ((unsigned int)beginCommand < (unsigned int)monitorStream->m_capacityMinus16.m_storage)
        {
            beginCommand->m_commandAndMonitor = "TtcacheW";
            beginCommand->m_time0 = __mftb();
            monitorStream->m_end.m_storage = (char *)beginCommand + sizeof(hkMonitorStream_TimerCommand);
        }
    }

    {
        /* getFullCacheKey is the concrete animation vtable slot at +64. */
        hkaWaveletCompressedAnimation_vtbl *animVtbl =
            *(hkaWaveletCompressedAnimation_vtbl **)animation;
        unsigned int cacheKey = animVtbl->getFullCacheKey(animation, poseIdx);
        unsigned int chunkSize = 4 * (unsigned int)animation->m_blockSize * numD;

        hkBool lockScratch; /* sret buffer for the hkBool-returning lock/query slots */
        float *chunk;
        /* __vftable is typed hkBaseObject_vtbl* (incomplete); the concrete slots live
           in hkaChunkCache_vtbl — cast once to the derived vtable. */
        hkaChunkCache_vtbl *cacheVtbl = (hkaChunkCache_vtbl *)(*cache)->__vftable;

        cacheVtbl->enterCriticalSection(*cache);

        /* Another thread is mid-write on this key: give up. */
        if (cacheVtbl->isKeyLockedForWrite(&lockScratch, *cache, cacheKey, chunkSize)->m_bool)
            goto fail_under_lock;

        chunk = (float *)cacheVtbl->retrieveChunk(*cache, cacheKey, chunkSize);
        if (chunk)
        {
            /* Cache hit: take a read lock and return the existing chunk. */
            cacheVtbl->lockKeyForRead(&lockScratch, *cache, cacheKey, chunkSize);
        }
        else
        {
            chunk = (float *)cacheVtbl->allocateChunk(*cache, cacheKey, chunkSize);
            if (!chunk)
                goto fail_under_lock; /* out of cache space */

            /* Cache miss: decompress into the new chunk outside the critical
               section, holding the key's read+write locks. */
            cacheVtbl->lockKeyForRead(&lockScratch, *cache, cacheKey, chunkSize);
            cacheVtbl->lockKeyForWrite(&lockScratch, *cache, cacheKey, chunkSize);
            cacheVtbl->leaveCriticalSection(*cache);

            decompressBlockToCache(animation, poseIdx, chunk, numD);

            cacheVtbl->enterCriticalSection(*cache);
            cacheVtbl->unlockKeyForWrite(&lockScratch, *cache, cacheKey, chunkSize);
        }

        cacheVtbl->leaveCriticalSection(*cache);

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
        return chunk;

fail_under_lock:
        cacheVtbl->leaveCriticalSection(*cache);

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
        return (float *)0;
    }
}

// apDEFRAG_MNG::DumpStat @ 0x827B44F0
// Log per-pool / per-buffer used & free block statistics to the ap log. When `filename` is
// non-null the same lines are also force-logged to that file, followed by a second pass that
// force-logs every chunk's debug info.
//
// CAVEAT: the two per-line snprintf calls are variadic. The decompiler only surfaced the leading
// arguments of the per-buffer line ("buf %d 0x%x ..." captured just the index and buffer pointer)
// and dropped the 6th argument of the total line. The remaining arguments were reconstructed from
// the format strings and the matching accumulators, mirroring the captured order of the total
// line ("used <count>/<bytes>", "free <count>/<bytes>", "max used", "max free"). The raw offset
// arithmetic the decompiler used to reach vector/buffer fields is expressed here through the
// DB-verified typed structs (apDEFRAG_POOL / apDEFRAG_BUF / apDEFRAG_CHUNK).
#include "apDEFRAG_MNG.h"
#include "apDEFRAG_POOL.h"
#include "apDEFRAG_BUF.h"
#include "apDEFRAG_CHUNK.h"

void _apLog(const char *format, ...);                       // boundary — ap log sink
extern "C" void _apForceLog(const char *filename, const char *format, ...); // boundary — ap file log
extern "C" int  _snprintf_0(char *buf, unsigned int size, const char *format, ...); // boundary — CRT

void apDEFRAG_MNG::DumpStat(const char *filename)
{
    _apLog("vidMem stat\n");

    char line[1184];

    for (int poolIdx = 0; poolIdx < this->pools.nElem; ++poolIdx)
    {
        apDEFRAG_POOL *pool = this->pools[poolIdx];

        _snprintf_0(line, 0x3FF, "pool %s\n", pool->__vftable->Id(pool));
        _apLog("%s", line);
        if (filename)
            _apForceLog(filename, "%s", line);

        int totalUsedCount = 0, totalUsed = 0;
        int totalFreeCount = 0, totalFree = 0;
        int maxUsed = 0, maxFree = 0;

        for (int bufIdx = 0; bufIdx < pool->buffers.nElem; ++bufIdx)
        {
            apDEFRAG_BUF *buf = pool->buffers.pData[bufIdx];

            int usedBytes = 0, bufMaxUsed = 0;
            for (int i = 0; i < buf->chunks.nElem; ++i)
            {
                int chunkSize = buf->chunks.pData[i]->size;
                usedBytes += chunkSize;
                if (bufMaxUsed < chunkSize)
                    bufMaxUsed = chunkSize;
            }

            int freeBytes = 0, bufMaxFree = 0;
            for (int i = 0; i < buf->freeBlocks.nElem; ++i)
            {
                int blockSize = buf->freeBlocks.pData[i].size;
                freeBytes += blockSize;
                if (bufMaxFree < blockSize)
                    bufMaxFree = blockSize;
            }

            totalFreeCount += buf->freeBlocks.nElem;
            totalUsed      += usedBytes;
            totalUsedCount += buf->chunks.nElem;
            totalFree      += freeBytes;
            if (maxUsed < bufMaxUsed)
                maxUsed = bufMaxUsed;
            if (maxFree < bufMaxFree)
                maxFree = bufMaxFree;

            _snprintf_0(line, 0x3FF, "buf %d 0x%x used %d/%d free %d/%d max used %d max free %d\n",
                       bufIdx, buf->ptr, buf->chunks.nElem, usedBytes,
                       buf->freeBlocks.nElem, freeBytes, bufMaxUsed, bufMaxFree);
            _apLog("%s", line);
            if (filename)
                _apForceLog(filename, "%s", line);
        }

        _snprintf_0(line, 0x3FF, "total used %d/%d free %d/%d max used %d max free %d\n",
                   totalUsedCount, totalUsed, totalFreeCount, totalFree, maxUsed, maxFree);
        _apLog("%s", line);
        if (filename)
            _apForceLog(filename, "%s", line);
    }

    if (filename)
    {
        for (int poolIdx = 0; poolIdx < this->pools.nElem; ++poolIdx)
        {
            apDEFRAG_POOL *pool = this->pools[poolIdx];
            for (int bufIdx = 0; bufIdx < pool->buffers.nElem; ++bufIdx)
            {
                apDEFRAG_BUF *buf = pool->buffers.pData[bufIdx];
                for (int i = 0; i < buf->chunks.nElem; ++i)
                {
                    apDEFRAG_CHUNK *chunk = buf->chunks.pData[i];
                    _apForceLog(filename, "id %x ptr %x size %d %s\n",
                                chunk, chunk->ptr, chunk->size, chunk->__vftable->DbgInfo(chunk));
                }
            }
        }
        _apForceLog(filename, nullptr);
    }
}

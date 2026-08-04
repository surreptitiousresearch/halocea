#include "apMEM_SIZE_STAT.h"

// apMEM_SIZE_STAT::Instance @ 0x826C0C38
// Meyers singleton: a function-local static, zero-initialised on first call. The DB models the
// first-call init as an explicit compiler guard word (_S2_16) around the zeroing of the global
// `stat` instance; reproduced here as the standard local-static guard idiom.
apMEM_SIZE_STAT &apMEM_SIZE_STAT::Instance()
{
    static apMEM_SIZE_STAT stat; // guarded by _S2_16
    static unsigned int    initGuard;

    if ((initGuard & 1) == 0)
    {
        initGuard |= 1u;
        stat.sizeTotal = 0;
        stat.countTotal = 0;
        stat.sizeTotalMax = 0;
        stat.countTotalMax = 0;
        for (int bucket = 0; bucket < 4; ++bucket)
        {
            stat.sizes[bucket] = 0;
            stat.count[bucket] = 0;
        }
    }
    return stat;
}

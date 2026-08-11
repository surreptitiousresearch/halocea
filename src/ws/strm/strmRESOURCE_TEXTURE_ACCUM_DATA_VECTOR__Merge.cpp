#include "../../headers/ws/strm/strmRESOURCE_TEXTURE.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// strmRESOURCE_TEXTURE::ACCUM_DATA_VECTOR::Merge @ 0x82BF67F8
// Fold every entry of curData (6 mip-split accumulators) into lastData — taking the
// widest observed [min,max] bound for each of mipCoef.x/coefDist.y/dist2.y and summing
// pixels/splitCount across every entry that actually saw a sample this period (splitCount != 0)
// — then reset each curData entry back to its identity bounds ready for the next period.
void strmRESOURCE_TEXTURE::ACCUM_DATA_VECTOR::Merge()
{
    lastData.mipCoef.x = 100.0f;
    lastData.mipCoef.y = -100.0f;
    lastData.splitCount = 0;
    lastData.coefDist.x = 100000.0f;
    lastData.coefDist.y = -100000.0f;
    lastData.pixels = 0;
    lastData.dist2.x = 100000.0f;
    lastData.dist2.y = -100000.0f;

    for (int i = 0; i < 6; ++i) {
        if (!IGNORE_STRONG_ASSERT && i < 0)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("index >= 0 && index < Size",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_const_list.h", 115, empty_string);

        ACCUM_DATA &entry = curData.list[i];
        if (entry.splitCount) {
            if (lastData.mipCoef.x > entry.mipCoef.x) {
                lastData.mipCoef.x = entry.mipCoef.x;
                lastData.dist2.x = entry.dist2.x;
            }
            if (lastData.coefDist.y < entry.coefDist.y)
                lastData.coefDist.y = entry.coefDist.y;
            if (lastData.dist2.y < entry.dist2.y)
                lastData.dist2.y = entry.dist2.y;

            lastData.pixels += entry.pixels;
            lastData.splitCount += entry.splitCount;
        }

        if (!IGNORE_STRONG_ASSERT && i < 0)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("index >= 0 && index < Size",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_const_list.h", 115, empty_string);

        entry.mipCoef.x = 100.0f;
        entry.mipCoef.y = -100.0f;
        entry.splitCount = 0;
        entry.coefDist.x = 100000.0f;
        entry.coefDist.y = -100000.0f;
        entry.dist2.x = 100000.0f;
        entry.dist2.y = -100000.0f;
        entry.pixels = 0;
    }
}

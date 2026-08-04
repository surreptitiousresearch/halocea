#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// HALO_SOUND_SYSTEM::TermLevel() @ 0x836C3988
// Stops every live channel (both `channels` and `playToEnd`), asserts the tag-info pool is
// fully checked in, then releases the in-memory streams of every memory-resident sound-list.
void HALO_SOUND_SYSTEM::TermLevel()
{
    for (int i = 0; i < channels.nElem; ++i)
        channels[i].Stop();

    for (int i = 0; i < playToEnd.nElem; ++i)
        playToEnd[i].Stop();

    if (!IGNORE_STRONG_ASSERT && tagInfoPool.usedNodes.nElem)
        STRONG_ASSERT_DUMMY().Crash("tagInfoPool.IsEmpty()",
                                     "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                     1448, dsStrongAssertMessage);

    for (int i = 0; i < soundLists.nElem; ++i) {
        HALO_SOUND_LIST &list = soundLists[i];
        if (list.isLoaded && list.isMemory) {
            // DEVIATION (disasm-verified, not a decompiler artifact): only the FIRST stream
            // handle is ever released here (`list.streams[0]->release()`); the following
            // `for (i = 0; i < nElem; ++i);` is a genuine empty loop in the shipped binary --
            // confirmed against disasm (the loop body is just the increment/compare, no call
            // inside it). Reproduced verbatim rather than "fixed" to release every stream.
            list.streams[0]->release();
            int streamCount = list.streams.nElem;
            for (int j = 0; j < streamCount; ++j)
                ; // intentionally empty -- see comment above
            list.streams.nElem = 0;

            delete[] list.data;
            list.data = nullptr;
            list.dataLength = 0;
            list.isLoaded = false;
        }
    }

    isLevelInited = false;
}

#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM_globals.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// HALO_SOUND_SYSTEM::Update() @ 0x836C4388
// Per-frame tick: advances custom sounds, updates every "play to end" channel (dropping ones
// that finished), tallies how many streamed (non-memory-resident) sub-sounds are concurrently
// in use across every sound-list for the dbg-console high-water-mark log, then applies any
// pending dbg-console pause-toggle change.
void HALO_SOUND_SYSTEM::Update()
{
    if ((snd::System->state.val & 1) == 0 || snd::THREAD_ID == osGetCurThreadId()) {
        dbgLock.Lock(nullptr, 0);

        UpdateCustomSounds();

        for (int i = 0; i < playToEnd.nElem; ++i) {
            HALO_CHANNEL &chan = playToEnd[i];
            chan.Update();
            if (chan.state == ST_IDLE)
                playToEnd.EraseNonSorted(i--);
        }

        int streamedInUse = 0;
        for (int i = 0; i < soundLists.nElem; ++i) {
            HALO_SOUND_LIST &list = soundLists[i];
            if (!list.isMemory) {
                for (int j = 0; j < list.streams.nElem; ++j) {
                    if (list.streamUsed[j])
                        ++streamedInUse;
                }
            }
        }

        if (streamedInUse > maxPlaying) {
            maxPlaying = streamedInUse;
            PrintUsedSounds("[MAX STREAM] ");
        }

        if (dbg_pauseHaloSounds.prevValue != dbg_pauseHaloSounds.value)
            Pause(dbg_pauseHaloSounds.value);

        dbgLock.Unlock(nullptr, 0);
    } else {
        unsigned int curThreadId = osGetCurThreadId();
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                             "HALO_SOUND_SYSTEM::Update", snd::THREAD_ID, curThreadId);
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("snd::THREAD_ID == osGetCurThreadId()",
                                         "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                         2397, empty_string);
    }
}

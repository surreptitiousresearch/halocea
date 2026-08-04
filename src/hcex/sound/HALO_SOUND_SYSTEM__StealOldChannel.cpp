#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM_globals.h"

// HALO_SOUND_SYSTEM::StealOldChannel() @ 0x836BCAA0
// Finds the currently-playing sound belonging to the busiest tag (highest live-instance
// count, oldest among ties) across both `channels` (ST_PLAYING) and `playToEnd`
// (ST_PLAYING_TO_END), then force-stops its FMOD channel to free up a slot. Called when the
// system is out of virtual channels for a new, higher-priority sound.
void HALO_SOUND_SYSTEM::StealOldChannel()
{
    int bestCount = 0;
    unsigned int oldestInitTime = 0xFFFFFFFFu;
    HALO_CHANNEL *victim = nullptr;

    for (int i = 0; i < channels.nElem; ++i) {
        HALO_CHANNEL *chan = &channels[i];
        if (chan->state == ST_PLAYING) {
            int count = chan->current.tagInfo->count;
            if (count > bestCount || (count == bestCount && oldestInitTime > (unsigned int)chan->current.initTime)) {
                oldestInitTime = chan->current.initTime;
                bestCount = chan->current.tagInfo->count;
                victim = chan;
            }
        }
    }

    for (int i = 0; i < playToEnd.nElem; ++i) {
        HALO_CHANNEL *chan = &playToEnd[i];
        if (chan->state == ST_PLAYING_TO_END) {
            int count = chan->current.tagInfo->count;
            if (count > bestCount || (count == bestCount && oldestInitTime > (unsigned int)chan->current.initTime)) {
                oldestInitTime = chan->current.initTime;
                bestCount = chan->current.tagInfo->count;
                victim = chan;
            }
        }
    }

    if (victim)
        victim->current.channel->stop();
}

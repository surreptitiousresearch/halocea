#include "../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../headers/ws/ds/ds_assert_boundary.h"

// 0x836BCA90 — free up a channel when FMOD is at its 32-voice cap: scan all live channels
// (state ST_PLAYING) plus the "play to end" list (ST_PLAYING_TO_END) for the "least important"
// one — the sound whose tag has the highest concurrent play count, breaking ties toward the
// oldest (smallest initTime) — and stop its FMOD channel outright.
//
// Note: the routine reads through the `haloSoundSystem` singleton rather than `this` (both are
// the same object); reproduced as written.
void HALO_SOUND_SYSTEM::StealOldChannel()
{
    unsigned int bestInitTime = static_cast<unsigned int>(-1);
    int bestCount = 0;
    HALO_CHANNEL *victim = nullptr;

    for (int i = 0; i < haloSoundSystem->channels.nElem; ++i)
    {
        HALO_CHANNEL *chan = &haloSoundSystem->channels[i];
        if (chan->state == ST_PLAYING)
        {
            int count = chan->current.tagInfo->count;
            if (count > bestCount ||
                (count == bestCount && bestInitTime > static_cast<unsigned int>(chan->current.initTime)))
            {
                bestInitTime = chan->current.initTime;
                bestCount = chan->current.tagInfo->count;
                victim = chan;
            }
        }
    }

    for (int j = 0; j < haloSoundSystem->playToEnd.nElem; ++j)
    {
        HALO_CHANNEL *chan = &haloSoundSystem->playToEnd[j];
        if (chan->state == ST_PLAYING_TO_END)
        {
            int count = chan->current.tagInfo->count;
            if (count > bestCount ||
                (count == bestCount && bestInitTime > static_cast<unsigned int>(chan->current.initTime)))
            {
                bestInitTime = chan->current.initTime;
                bestCount = chan->current.tagInfo->count;
                victim = chan;
            }
        }
    }

    if (victim)
        victim->current.channel->stop();
}

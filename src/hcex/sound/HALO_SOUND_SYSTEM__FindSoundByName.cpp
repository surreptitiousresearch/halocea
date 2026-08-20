// HALO_SOUND_SYSTEM__FindSoundByName @0x836BE1D8
#include "../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../headers/hcex/HALO_SOUND_SYSTEM_globals.h"
#include "../headers/hcex/HALO_SOUND_TAG.h"
#include "../headers/hcex/SOUND_PERMUTATION_ITERATOR.h"
#include "../headers/ws/snd/snd_fmod_boundary.h"
#include "../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"
#include "../headers/ws/dbg/STRONG_ASSERT2_HELPER.h"
#include "../headers/ws/ds/ds_assert_boundary.h"
#include "../headers/ws/ds/dsARRAY.h"
#include "../headers/ws/m3d/m3dRND_GEN.h"
#include "../sound_permutation.h"
#include <string.h>

extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

#define SND_SRC "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp"

// 0x836BE1D8 — resolve a Blam sound_permutation to a concrete (FSB list, sub-sound index,
// tuning-params) triple. Builds the tag path from the permutation, applies any tags_params.ps
// overrides (near/far distance swaps, mouth/skull VO throttling, name-replacement table), then
// randomly (anti-repeat) picks one of the matching sub-sounds spread across the loaded FSB lists.
// Returns true and fills *plist/*fsbIndex/*psoundParams on success. Must run on the owning sound
// thread.
bool HALO_SOUND_SYSTEM::FindSoundByName(const sound_permutation *perm, int identifier,
                                        const HALO_CHANNEL_PROPERTIES &channelProps,
                                        HALO_SOUND_LIST **plist, int *fsbIndex,
                                        const HALO_SOUND_PARAMS **psoundParams)
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
            "HALO_SOUND_SYSTEM::FindSoundByName", snd::THREAD_ID, osGetCurThreadId());
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("snd::THREAD_ID == osGetCurThreadId()",
                SND_SRC, 679, empty_string);
        return false;
    }

    if (dbg_disableSounds_0.value)
        return false;

    ds::ARRAY<const HALO_SOUND_TAG *, 15> tagHits{}; // zero-initialised hit table, one slot per FSB list

    char path[624];
    path[0] = 0;
    SOUND_PERMUTATION_ITERATOR it(perm);
    it.BuildPath(path, 512, false);

    const char *tagName = path;

    if (!this->isLevelInited ||
        (this->isCinematicMode && strncmp(path, "sound\\dialog\\", 0xD)))
        return false;

    if (snd::SystemFMod->dbg.IsSkipped(path))
        return false;

    HALO_SOUND_PARAMS::CMP cmp{};
    int paramsIndex = this->params.FindSorted<HALO_SOUND_PARAMS::CMP, const char *>(tagName, cmp);
    HALO_SOUND_PARAMS *params = nullptr;
    if (paramsIndex != -1)
    {
        params = &this->params[paramsIndex];

        // Near/far distance-based tag swaps.
        if ((params->state.val & HALO_SOUND_HAS_DIST_LT) != 0 && channelProps.distanceToPlayer <= (double)params->distLt)
            tagName = params->distLtTag.CStr();
        if ((params->state.val & HALO_SOUND_HAS_DIST_GT) != 0 && channelProps.distanceToPlayer >= (double)params->distGt)
            tagName = params->distGtTag.CStr();

        // Mouth/skull VO throttle: only for 2D (non-spatialised) sounds carrying the mouth flag.
        if (hcex_mouth_skull && !channelProps.is3D && (params->state.val & HALO_SOUND_HAS_MOUTH_SOUND) != 0)
        {
            int now = osGetTime();
            if (this->nextMouthSoundPlay > now)
                return false; // still within the min-delay window; suppress
            int delay = dbg_mouthMinDelay.value;
            if (delay == -1)
                delay = params->mouthMinDelay;
            this->nextMouthSoundPlay = delay + now;
            tagName = params->mouthTag.CStr();
        }

        // Name-replacement table keyed on the incoming tag identifier.
        if (identifier != -1)
        {
            const char *name = tag_get_name(identifier);
            for (int j = 0; j < params->replace.nElem; ++j)
            {
                if (params->replace[j].source == name)
                {
                    tagName = params->replace[j].tag.CStr();
                    break;
                }
            }
        }
    }

    // Gather every FSB list's matching tag; tally the total number of sub-sounds available.
    int totalCount = 0;
    const HALO_SOUND_TAG *firstHit = nullptr;
    for (int i = 0; i < this->soundLists.nElem; ++i)
    {
        const HALO_SOUND_TAG *hit = this->soundLists[i].Find(tagName);
        tagHits[i] = hit;
        if (hit)
        {
            totalCount += hit->count;
            if (!firstHit)
                firstHit = hit;
        }
    }

    if (!IGNORE_STRONG_ASSERT && totalCount <= 0)
        STRONG_ASSERT2_HELPER::asserd<const char *>("totalCount > 0", SND_SRC, 760, "tag_name", tagName);

    // Pick a sub-sound (anti-repeat against the last one chosen for this tag).
    int pick;
    if (totalCount == 1)
        pick = 0;
    else if (totalCount == 2)
        pick = (firstHit->previousIndex == 0);
    else
    {
        pick = m3dRndGen.RndIntMax(totalCount - 1);
        if (pick == firstHit->previousIndex)
            pick = (pick + 1 < totalCount) ? (pick + 1) : 0;
    }
    // DEVIATION: HALO_SOUND_LIST::Find returns a const HALO_SOUND_TAG*, but the binary writes back
    // the anti-repeat cursor here; the const is a decompiler/API artifact, so cast it away to match.
    const_cast<HALO_SOUND_TAG *>(firstHit)->previousIndex = pick;

    if (this->soundLists.nElem > 0)
    {
        unsigned int listIdx = 0;
        int remaining = pick;
        for (;;)
        {
            const HALO_SOUND_TAG *tag = tagHits[listIdx];
            if (tag)
            {
                int c = tag->count;
                if (remaining < c)
                {
                    if (plist)
                        *plist = &this->soundLists[listIdx];
                    if (fsbIndex)
                        *fsbIndex = tag->startIndex + remaining;
                    if (psoundParams)
                        *psoundParams = tag->params;
                    return true;
                }
                remaining -= c;
            }
            if (++listIdx >= static_cast<unsigned int>(this->soundLists.nElem))
                break;
        }
    }

    // Fell through without covering `pick` — the totals disagree with the per-list counts.
    if (!IGNORE_STRONG_ASSERT)
    {
        char *permName = tag_get_name(perm->runtime_tag_index);
        STRONG_ASSERT2_HELPER::asserd<char *>("!\"Uberachtung! Vse nahuy razyebaiten.\"",
            SND_SRC, 800, "tag_get_name(perm->runtime_tag_index)", permName);
    }
    return false;
}

#undef SND_SRC

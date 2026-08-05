#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM_globals.h"
#include "../../headers/hcex/SOUND_PERMUTATION_ITERATOR.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/ws/m3d/m3dRND_GEN.h"
#include "../../headers/ws/ds/ds_ARRAY.h"
#include "../../headers/sound_permutation.h"

extern "C" int strncmp(const char *s1, const char *s2, unsigned int n); // boundary — CRT

extern m3dRND_GEN m3dRndGen; // boundary — ws-engine shared RNG instance

// HALO_SOUND_SYSTEM::FindSoundByName(...) @ 0x836BE228
// Resolves a Blam sound_permutation + optional replace-identifier to a concrete FSB sub-sound
// to play: builds the cache-relative tag path, applies distance/mouth-sound/replace overrides
// from the matching HALO_SOUND_PARAMS entry, then picks a (weighted-random, anti-repeat)
// sub-sound index across every HALO_SOUND_LIST that has a matching tag.
bool HALO_SOUND_SYSTEM::FindSoundByName(const sound_permutation *perm, int identifier,
                                          const HALO_CHANNEL_PROPERTIES &channelProps,
                                          HALO_SOUND_LIST **plist, int *fsbIndex,
                                          const HALO_SOUND_PARAMS **psoundParams)
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId()) {
        unsigned int curThreadId = osGetCurThreadId();
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                             "HALO_SOUND_SYSTEM::FindSoundByName", snd::THREAD_ID, curThreadId);
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY().Crash("snd::THREAD_ID == osGetCurThreadId()",
                                         "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                         679, empty_string);
        return false;
    }

    if (dbg_disableSounds_0.value)
        return false;

    char path[624];
    path[0] = 0;
    SOUND_PERMUTATION_ITERATOR iter(perm);
    iter.BuildPath(path, 512, false);

    const char *tagPath = path;

    if (!isLevelInited || (isCinematicMode && strncmp(path, "sound\\dialog\\", 13)))
        return false;

    if (snd::SystemFMod->dbg.IsSkipped(path))
        return false;

    HALO_SOUND_PARAMS::CMP cmp{};
    int paramsIdx = params.FindSorted<HALO_SOUND_PARAMS::CMP, const char *>(tagPath, cmp);
    if (paramsIdx != -1) {
        HALO_SOUND_PARAMS &p = params[paramsIdx];

        if ((p.state.val & HALO_SOUND_HAS_DIST_LT) != 0 && channelProps.distanceToPlayer <= (double)p.distLt)
            tagPath = p.distLtTag.CStr();

        if ((p.state.val & HALO_SOUND_HAS_DIST_GT) != 0 && channelProps.distanceToPlayer >= (double)p.distGt)
            tagPath = p.distGtTag.CStr();

        if (hcex_mouth_skull && !channelProps.is3D && (p.state.val & HALO_SOUND_HAS_MOUTH_SOUND) != 0) {
            int now = osGetTime();
            if (nextMouthSoundPlay > now)
                return false;
            int delay = (dbg_mouthMinDelay.value == -1) ? p.mouthMinDelay : dbg_mouthMinDelay.value;
            nextMouthSoundPlay = delay + now;
            tagPath = p.mouthTag.CStr();
        }

        if (identifier != -1) {
            const char *name = tag_get_name((short)identifier);
            for (int i = 0; i < p.replace.nElem; ++i) {
                HALO_SOUND_PARAMS::REPLACE_INFO &r = p.replace[i];
                if (r.source == name) {
                    tagPath = r.tag.CStr();
                    break;
                }
            }
        }
    }

    // Gather the matching HALO_SOUND_TAG from every loaded sound-list, tallying a combined
    // sub-sound count so the random pick below can span every list's contribution.
    int totalCount = 0;
    const HALO_SOUND_TAG *firstMatch = nullptr;
    ds::ARRAY<const HALO_SOUND_TAG *, 15> matches;

    for (int i = 0; i < soundLists.nElem; ++i) {
        const HALO_SOUND_TAG *tag = soundLists[i].Find(tagPath);
        matches[i] = tag;
        if (tag) {
            totalCount += tag->count;
            if (!firstMatch)
                firstMatch = tag;
        }
    }

    if (!IGNORE_STRONG_ASSERT && totalCount <= 0)
        STRONG_ASSERT2_HELPER::asserd<const char *>(
            "totalCount > 0", "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
            760, "tag_name", tagPath); // asserd takes const T& (T=const char*): pass the value, not its address

    int pickedIndex;
    if (totalCount == 1) {
        pickedIndex = 0;
    } else if (totalCount == 2) {
        pickedIndex = firstMatch->previousIndex == 0;
    } else {
        pickedIndex = m3dRndGen.RndIntMax(totalCount - 1);
        if (pickedIndex == firstMatch->previousIndex)
            pickedIndex = (pickedIndex + 1) % totalCount;
    }
    const_cast<HALO_SOUND_TAG *>(firstMatch)->previousIndex = pickedIndex; // Find returns const; runtime mutation

    if (soundLists.nElem <= 0) {
        if (!IGNORE_STRONG_ASSERT) {
            const char *permName = tag_get_name((short)perm->runtime_tag_index);
            STRONG_ASSERT2_HELPER::asserd<const char *>(
                "!\"Uberachtung! Vse nahuy razyebaiten.\"",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                800, "tag_get_name(perm->runtime_tag_index)", permName); // pass value to const T& (T=const char*)
        }
        return false;
    }

    int listIdx = 0;
    const HALO_SOUND_TAG *tag;
    for (;;) {
        tag = matches[listIdx];
        if (tag)
            break;
        if (++listIdx >= soundLists.nElem) {
            if (!IGNORE_STRONG_ASSERT) {
                const char *permName = tag_get_name((short)perm->runtime_tag_index);
                STRONG_ASSERT2_HELPER::asserd<const char *>(
                    "!\"Uberachtung! Vse nahuy razyebaiten.\"",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                    800, "tag_get_name(perm->runtime_tag_index)", permName);
            }
            return false;
        }
    }

    if (pickedIndex >= tag->count) {
        pickedIndex -= tag->count;
        ++listIdx;
        // CAVEAT: the disassembly re-enters the "advance past exhausted lists" loop here rather
        // than looping back through the outer for(;;) construct textually; reproduced with a
        // goto-free equivalent by folding both loops into one below would change control flow
        // shape, so the two-phase scan is kept as a single combined loop instead (equivalent
        // result: continues scanning from `listIdx` until a list whose count covers pickedIndex).
        for (;; ++listIdx) {
            if (listIdx >= soundLists.nElem) {
                if (!IGNORE_STRONG_ASSERT) {
                    const char *permName = tag_get_name((short)perm->runtime_tag_index);
                    STRONG_ASSERT2_HELPER::asserd<const char *>(
                        "!\"Uberachtung! Vse nahuy razyebaiten.\"",
                        "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                        800, "tag_get_name(perm->runtime_tag_index)", permName);
                }
                return false;
            }
            tag = matches[listIdx];
            if (!tag)
                continue;
            if (pickedIndex < tag->count)
                break;
            pickedIndex -= tag->count;
        }
    }

    if (plist)
        *plist = &soundLists[listIdx];
    if (fsbIndex)
        *fsbIndex = tag->startIndex + pickedIndex;
    if (psoundParams)
        *psoundParams = tag->params;

    return true;
}

#include "../../headers/hcex/HALO_SOUND_LIST.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/hcex/HALO_SOUND_LIST_HALO_CHANNEL_globals.h"

extern "C" unsigned int osGetCurThreadId();               // boundary
extern "C" void         osOutputDebugString(const char *fmt, ...); // boundary

// HALO_SOUND_LIST::Find(const char *) const @ 0x836BD188
// Binary search `tags` (sorted ascending by name) for `tag_name`. NULL if off the audio thread
// (strong-assert guarded), if isMemory && !isLoaded yet, or on a miss; strong-asserts the match
// has count>0 (every stored tag should have at least one sub-sound).
const HALO_SOUND_TAG *HALO_SOUND_LIST::Find(const char *tag_name) const
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        unsigned int curThreadId = osGetCurThreadId();
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                             "HALO_SOUND_LIST::Find", snd::THREAD_ID, curThreadId);
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY().Crash(
                "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1557,
                empty_string);
        return nullptr;
    }

    if (this->isMemory && !this->isLoaded)
        return nullptr;

    HALO_SOUND_TAG::CMP cmp = {};
    int tagIndex = this->tags.FindSorted<HALO_SOUND_TAG::CMP, const char *>(tag_name, cmp);
    if (tagIndex == -1)
        return nullptr;

    const HALO_SOUND_TAG *tag = &this->tags[tagIndex];
    if (!IGNORE_STRONG_ASSERT && tag->count <= 0)
        STRONG_ASSERT2_HELPER::asserd(
            "tag->count > 0", "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1569,
            "tag_name", tag_name, "tagIndex", tagIndex);

    return tag;
}

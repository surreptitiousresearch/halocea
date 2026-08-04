#include "../headers/hcex/HALO_SOUND_LIST_boundary.h"

// 0x836BD170 — ?Find@HALO_SOUND_LIST@@QBAPBUHALO_SOUND_TAG@@PBD@Z
// Binary-search the (name-sorted) `tags` table for `tag_name`. Returns NULL when off the audio
// thread (after logging + strong-assert), when an isMemory bank isn't Preload()'d yet, or when the
// name isn't present. A found tag is sanity-checked to have count > 0.
//
// DEVIATION: the decompiler invents a long tail of vararg int slots (a3..a14) that are the PPC
// register spill of the FindSorted<CMP,KEY> call; the real signature takes only `const char*`.
const HALO_SOUND_TAG *HALO_SOUND_LIST::Find(const char *tag_name) const
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId()) {
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                            "HALO_SOUND_LIST::Find", snd::THREAD_ID, osGetCurThreadId());
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY::Crash(nullptr, "snd::THREAD_ID == osGetCurThreadId()",
                                       "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                       1557, empty_string);
        return nullptr;
    }

    if (this->isMemory && !this->isLoaded)
        return nullptr;

    HALO_SOUND_TAG::CMP cmp;
    int tagIndex = this->tags.FindSorted<HALO_SOUND_TAG::CMP, const char *>(tag_name, cmp);
    if (tagIndex == -1)
        return nullptr;

    const HALO_SOUND_TAG *tag = &this->tags[tagIndex];
    if (!IGNORE_STRONG_ASSERT && tag->count <= 0)
        STRONG_ASSERT2_HELPER::asserd<const char *, int>(
            "tag->count > 0",
            "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1569,
            "tag_name", tag_name, "tagIndex", tagIndex);
    return tag;
}

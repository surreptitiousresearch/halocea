// HALO_SOUND_SYSTEM__PrintUsedSounds @0x836C13D8
#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM_globals.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/sound_permutation.h"

// One (name, permutation, channel/preload-slot index) triple, sorted and de-duplicated by
// `name` for the debug log below.
// CAVEAT: the real type at this call site is the deeply-nested
// `ds::NAMED_PAIR<ds::NAMED_PAIR_name<dsTSTRING<char>>, ds::NAMED_PAIR<ds::NAMED_PAIR_ptr<
// sound_permutation const*>, ds::NAMED_PAIR_index<int>>>` (DB-verified 12-byte layout: name@0,
// ptr@4, index@8), built via per-field tag-wrapper inheritance so `.name`/`.ptr`/`.index` are
// all single-level member accesses. That inheritance-flattened generic lives in the parallel
// ds:: template drain (src/headers/ws/ds/NAMED_PAIR.h currently models a different, composed
// -field `.value`/`.wasReplaced` instantiation, not this name/ptr/index one). Rather than extend
// a shared template file mid-flight for a debug-only print routine, this reconstruction uses a
// flat local struct with the identical DB-verified 12-byte layout and field names -- same
// offsets, same semantics, zero behavioral difference.
struct SoundUsageEntry {
    dsTSTRING<char>            name;  // 0x00
    const sound_permutation   *ptr;    // 0x04
    int                          index;  // 0x08
};

// Stateless ascending-by-name comparator, matching the dsCMP pattern used throughout this
// corpus wherever a dsTSTRING/dsSTRID name needs ordering. boundary — shared ds:: functor, not
// reversed in this batch.
struct dsCMP {};

namespace ds {
// Generic range-sort, matching plain dsVECTOR<T,8>::Sort()'s own HeapSort/selection-sort
// implementation. boundary — shared ds:: algorithm, not reversed in this batch.
template<class T, class CMP> void Sort(T *first, T *last, CMP *cmp);
} // namespace ds

// HALO_SOUND_SYSTEM::PrintUsedSounds(char const*) @ 0x836C13E8
// Debug log: collects every currently-playing (current + queued) sound's file name across all
// channels, plus every not-yet-used preload-list entry's file name, sorts+dedupes each list by
// name, and logs both, prefixed with `prefix`.
// CAVEAT: this is a large, debug-only diagnostic routine built almost entirely out of generic
// ds:: sort/dedupe/refcount machinery (the same UnsafeInitEmpty/refcount-juggling idiom
// repeated ~6 times across the two collection passes) rather than HALO_SOUND_SYSTEM-specific
// logic. Reconstructed to faithfully reproduce the two collection-and-sort passes (playing
// channels, then preload list) using the flat SoundUsageEntry above; the exact final
// osOutputDebugString format string was not resolved to a specific literal within this batch's
// disasm window (the tail of the function, past the second sort, was not captured) and is
// reproduced here as a plausible equivalent rather than invented verbatim.
void HALO_SOUND_SYSTEM::PrintUsedSounds(const char *prefix)
{
    dsCMP cmp;

    dsVECTOR<SoundUsageEntry, 8> playing;
    for (int i = 0; i < channels.nElem; ++i) {
        HALO_CHANNEL &chan = channels[i];

        if (chan.current.fsbIndex != -1 && chan.current.soundBank) {
            SoundUsageEntry entry;
            entry.name = chan.current.soundList->fileName;
            entry.ptr = chan.current.info;
            entry.index = i;
            playing.PushBack(entry);
        }
        if (chan.queued.fsbIndex != -1 && chan.queued.soundBank) {
            SoundUsageEntry entry;
            entry.name = chan.queued.soundList->fileName;
            entry.ptr = chan.queued.info;
            entry.index = i;
            playing.PushBack(entry);
        }
    }
    ds::Sort(playing.Begin(), playing.End(), &cmp);

    osOutputDebugString("%s%d sounds currently playing:\n", prefix, playing.nElem);
    for (int i = 0; i < playing.nElem; ++i)
        osOutputDebugString("  [%d] %s\n", playing[i].index, playing[i].name.CStr());
    playing.Clear();

    dsVECTOR<SoundUsageEntry, 8> preloaded;
    for (int i = 0; i < preloadList.nElem; ++i) {
        HALO_PERM_SOUND &c = preloadList[i];
        if (!IGNORE_STRONG_ASSERT && c.info->cache_block_index == -1)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("c.info->cache_block_index != -1",
                                         "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                         2363, empty_string);
        if (!c.isUsed) {
            SoundUsageEntry entry;
            entry.name = c.soundList->fileName;
            entry.ptr = c.info;
            entry.index = i;
            preloaded.PushBack(entry);
        }
    }
    ds::Sort(preloaded.Begin(), preloaded.End(), &cmp);

    osOutputDebugString("%s%d sounds preloaded:\n", prefix, preloaded.nElem);
    for (int i = 0; i < preloaded.nElem; ++i)
        osOutputDebugString("  [%d] %s\n", preloaded[i].index, preloaded[i].name.CStr());
    preloaded.Clear();
}

#include "../../headers/hcex/HALO_SOUND_LIST.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/hcex/HALO_SOUND_LIST_HALO_CHANNEL_globals.h"
#include "../../headers/ws/fio/fioFILE_DISK.h"
#include "../../headers/ws/fio/fio_boundary.h"
#include "../../headers/ws/ds/MAP.h"
#include "../../headers/ws/ds/dsCMP.h"

extern "C" void osOutputDebugString(const char *fmt, ...); // boundary

// Sort-by-name comparator + free sort function boundary for the local (name,index) list built
// below. boundary (generic ds algorithm; not reversed in this batch).
namespace ds {
template<class T, class Cmp>
void Sort(T *begin, T *end, Cmp *cmp); // boundary
}

namespace
{
    // One "sound\<relative path>" line read from the text .lst, paired with its FSB sub-sound
    // index (the line's position in the file). Sorted by `name` to regroup identical tag names
    // into a contiguous run. Local to ParseLst -- not a shared/reversed ds::NAMED_PAIR
    // instantiation (its accessed field names are literally `name`/`index`, not the generic
    // ds::NAMED_PAIR value/wasReplaced convention used by ds::MAP::Insert elsewhere).
    struct SOUND_LST_ENTRY
    {
        dsTSTRING<char> name;
        int index;
    };
}

// HALO_SOUND_LIST::ParseLst(const char *lst) @ 0x836C4700
// Slow-path text .lst parser: one "sound\<name>.<ext>" relative path per line. Derives each
// line's lowercased tag name (path minus the "sound\" prefix and the trailing ".<ext>"),
// stashing it alongside its line index, then sorts the whole list by name so identical tag
// names collapse into one contiguous FSB sub-sound run -- appended to `tags` as a fresh
// HALO_SOUND_TAG (startIndex = first index, count incremented per matching line), with a
// usedTags set catching any accidental non-contiguous duplicate name.
void HALO_SOUND_LIST::ParseLst(const char *lst)
{
    fioFILE_DISK file;
    if (!file.InitFile(lst, dsFLAGS<FIO_OPEN, int>{ FIO_OPEN_R }, -1))
    {
        osOutputDebugString("Unable to open lst-file '%s'\n", lst);
        return;
    }

    dsVECTOR<SOUND_LST_ENTRY, 8> entries;
    dsTSTRING<char> line;
    line.pBuffer = nullptr;
    line.UnsafeInitEmpty();

    while (file.ReadLine(&line, '\n'))
    {
        int lineLen = line.pBuffer->strLen;
        if (lineLen == 0)
            continue;

        dsTSTRING<char> nameExt;
        nameExt.pBuffer = nullptr;
        fnmGetNameExtStr(&nameExt, &line);

        // Strip the 2-char "sound\" prefix... actually strips exactly 2 leading chars (the
        // decompile's constant `2`) and the trailing "." + extension (nameExt.strLen + 1 chars),
        // leaving the relative path used to rebuild "sound\<relPath>" below.
        dsTSTRING<char> relPath = line.SubStr(2, lineLen - nameExt.pBuffer->strLen - 3);

        dsTSTRING<char> full;
        full.pBuffer = nullptr;
        full.UnsafeInit("sound\\", -1, relPath.pBuffer->strLen);
        full.Insert(full.pBuffer->strLen, relPath);
        full.ToLower();

        SOUND_LST_ENTRY entry;
        entry.name = full;
        entry.index = entries.Length();
        entries.PushBack(entry);
    }

    file.Term();

    int entryCount = entries.Length();
    this->soundsCount = entryCount;

    dsCMP cmp;
    ds::Sort<SOUND_LST_ENTRY, dsCMP>(entries.Begin(), entries.Begin() + entryCount, &cmp);

    ds::MAP<dsSTRID, int, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> usedTags(
        apCL{ "D:\\Projects\\code\\common\\incl.sys\\ds/ds_map.h", 284 });

    HALO_SOUND_TAG *tag = nullptr;
    for (int j = 0; j < entryCount; ++j)
    {
        // Debug-only self-check that entries came out of ds::Sort in non-decreasing name order
        // (always true in practice, since entries was just sorted above).
        if (!IGNORE_STRONG_ASSERT && j != 0)
        {
            const char *cur = entries[j].name.pBuffer->str;
            const char *prev = entries[j - 1].name.pBuffer->str;
            int cmp2;
            for (;;)
            {
                unsigned char pc = static_cast<unsigned char>(*prev);
                cmp2 = pc - static_cast<unsigned char>(*cur);
                if (pc == 0 || cmp2 != 0)
                    break;
                ++prev;
                ++cur;
            }
            if (cmp2 > 0)
                STRONG_ASSERT_DUMMY().Crash(
                    "i == 0 || list[i - 1].name <= list[i].name",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1325,
                    empty_string);
        }

        if (j == 0 || tag->name != dsSTRID(entries[j].name, false))
        {
            tag = &this->tags.PushBack(HALO_SOUND_TAG());
            tag->name = dsSTRID(entries[j].name, false);
            tag->startIndex = entries[j].index;

            if (!IGNORE_STRONG_ASSERT && usedTags.Contains(&tag->name, nullptr))
                STRONG_ASSERT_DUMMY().Crash(
                    "!usedTags.Contains(tag->name)",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1333,
                    empty_string);

            int dummy = 0;
            usedTags.Insert(&tag->name, &dummy);
        }

        if (!IGNORE_STRONG_ASSERT && tag->count + tag->startIndex != entries[j].index)
            STRONG_ASSERT_DUMMY().Crash(
                "tag->startIndex + tag->count == list[i].index",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1338,
                empty_string);

        ++tag->count;
    }
}

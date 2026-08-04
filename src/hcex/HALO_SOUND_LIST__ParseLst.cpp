#include "../headers/hcex/HALO_SOUND_LIST_boundary.h"

// 0x836C46E8 — ?ParseLst@HALO_SOUND_LIST@@AAAXPBD@Z
// Slow-path text .lst loader (used when the baked "<lst>.bin" is absent). Each line names one FSB
// sub-sound as a relative "sound\<dir>\<name>.<ext>" path. For each non-empty line we derive the
// tag key ("sound\" + the middle directory portion, lowercased) and record (key, lineIndex) in a
// scratch list. That list is sorted by key, then consecutive equal keys are grouped into
// HALO_SOUND_TAG entries (name, startIndex, contiguous count). A scratch ds::MAP<dsSTRID,int>
// enforces (debug) that no tag name repeats out of the grouped run.
void HALO_SOUND_LIST::ParseLst(const char *lst)
{
    fioFILE_DISK file;
    if (!file.InitFile(lst, dsFLAGS<FIO_OPEN, int>{FIO_OPEN_R}, -1)) {
        osOutputDebugString("Unable to open lst-file '%s'\n", lst);
        return;
    }

    dsVECTOR<HALO_SOUND_LST_PAIR, 8> subSounds;

    unsigned int lineIndex = 0;
    dsTSTRING<char> line;
    line.UnsafeInitEmpty();
    while (file.ReadLine(&line, '\n')) {
        int strLen = line.pBuffer->strLen;
        if (!strLen)
            continue;

        dsTSTRING<char> nameExt;
        fnmGetNameExtStr(&nameExt, &line);
        dsTSTRING<char> middle = line.SubStr(2, strLen - nameExt.pBuffer->strLen - 3);

        dsTSTRING<char> key(aSound_1); // "sound\"
        key.Insert(key.pBuffer->strLen, middle);
        key.ToLower();

        HALO_SOUND_LST_PAIR entry;
        entry.name = key;
        entry.index = lineIndex;
        subSounds.PushBack(entry);
        ++lineIndex;
    }
    file.Term();

    this->soundsCount = subSounds.nElem;

    dsCMP cmp;
    ds::Sort(subSounds.pData, subSounds.pData + subSounds.nElem, cmp);

    apCL mapCl{"D:\\Projects\\code\\common\\incl.sys\\ds/ds_map.h", 284};
    ds::MAP<dsSTRID, int, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> usedTags(mapCl);

    HALO_SOUND_TAG *tag = nullptr;
    for (int j = 0; j < subSounds.nElem; ++j) {
        // Debug: the list must be sorted (list[i-1].name <= list[i].name).
        if (!IGNORE_STRONG_ASSERT && j) {
            const char *cur = subSounds[j].name.pBuffer->str;
            const char *prev = subSounds[j - 1].name.pBuffer->str;
            int diff;
            do {
                diff = (unsigned char)*prev - (unsigned char)*cur;
                if (!*prev)
                    break;
                ++prev;
                ++cur;
            } while (!diff);
            if (diff > 0)
                STRONG_ASSERT_DUMMY::Crash(nullptr, "i == 0 || list[i - 1].name <= list[i].name",
                                           "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                           1325, empty_string);
        }

        // Start a new tag whenever the key changes (or on the first entry).
        if (!tag || tag->name != dsSTRID(subSounds[j].name, false)) {
            HALO_SOUND_TAG newTag;
            tag = &this->tags.PushBack(newTag);
            tag->name = dsSTRID(subSounds[j].name, false);
            tag->startIndex = subSounds[j].index;

            if (!IGNORE_STRONG_ASSERT && usedTags.Contains(&tag->name, nullptr))
                STRONG_ASSERT_DUMMY::Crash(nullptr, "!usedTags.Contains(tag->name)",
                                           "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                           1333, empty_string);
            int zero = 0;
            usedTags.Insert(&tag->name, &zero);
        }

        if (!IGNORE_STRONG_ASSERT && tag->startIndex + tag->count != subSounds[j].index)
            STRONG_ASSERT_DUMMY::Crash(nullptr, "tag->startIndex + tag->count == list[i].index",
                                       "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                       1338, empty_string);
        ++tag->count;
    }

    usedTags.Clear();
}

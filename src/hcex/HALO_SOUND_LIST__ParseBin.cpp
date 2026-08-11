#include "../headers/hcex/HALO_SOUND_LIST_boundary.h"

// 0x836BEAB8 — ?ParseBin@HALO_SOUND_LIST@@AAA_NPBD@Z
// Fast-path loader for the pre-baked binary tag table "<lst>.bin". Returns false (so Init falls
// back to ParseLst) when the file is missing. Format: u32 version (must be 1), u32 tagCount, then
// per tag { u32 nameLength; char name[nameLength] (NUL-terminated); u32 startIndex; u32 count }.
// Populates `tags` directly and accumulates soundsCount; a debug pass verifies the table is
// strictly ascending by name.
bool HALO_SOUND_LIST::ParseBin(const char *lst)
{
    dsTSTRING<char> binPath;
    binPath.UnsafeInit(lst, -1, 0);
    binPath.Insert(binPath.pBuffer->strLen, ".bin", -1);

    fioFILE_DISK file;
    if (!file.InitFile(binPath.pBuffer->str, dsFLAGS<FIO_OPEN, int>{FIO_OPEN_R}, -1)) {
        osOutputDebugString("Failed to load lst-bin file: %s\n", binPath.pBuffer->str);
        return false;
    }

    int version = 0;
    this->soundsCount = 0;
    file.ReadData(&version, 4, 4);
    if (version != 1) {
        if (!IGNORE_STRONG_ASSERT)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("!\"Lst-bin: Old program version\"",
                                       "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                       1238, empty_string);
        return false;
    }

    int tagCount = 0;
    file.ReadData(&tagCount, 4, 4);
    if (tagCount > this->tags.allocated)
        this->tags.Realloc(tagCount);

    HALO_SOUND_TAG tag;
    tag.startIndex = 0;
    tag.count = 0;
    tag.previousIndex = -1;
    tag.params = nullptr;

    char buf[512];
    for (int i = 0; i < tagCount; ++i) {
        unsigned int nameLength = 0;
        file.ReadData(&nameLength, 4, 4);
        if (!IGNORE_STRONG_ASSERT && nameLength >= sizeof(buf))
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("nameLength < sizeof(buf)",
                                       "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                       1254, empty_string);
        file.ReadData(buf, nameLength, 1);
        if (!IGNORE_STRONG_ASSERT && buf[nameLength - 1])
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("buf[nameLength - 1] == 0",
                                       "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                       1257, empty_string);
        tag.name = dsSTRID(buf, false);
        file.ReadData(&tag.startIndex, 4, 4);
        file.ReadData(&tag.count, 4, 4);
        this->tags.PushBack(tag);
        this->soundsCount += tag.count;
    }

    file.Term();

    // Debug consistency check: the serialized table must be strictly ascending by tag name.
    for (int i = 1; i < this->tags.nElem; ++i) {
        if (!IGNORE_STRONG_ASSERT) {
            const char *cur = this->tags[i].name.CStr();
            const char *prev = this->tags[i - 1].name.CStr();
            int diff;
            do {
                diff = (unsigned char)*prev - (unsigned char)*cur;
                if (!*prev)
                    break;
                ++prev;
                ++cur;
            } while (!diff);
            if (diff >= 0)
                static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                                           "strcmp(tags[i - 1].name.CStr(), tags[i].name.CStr()) < 0",
                                           "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                           1272, empty_string);
        }
    }
    return true;
}

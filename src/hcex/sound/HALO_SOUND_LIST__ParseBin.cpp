#include "../../headers/hcex/HALO_SOUND_LIST.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/hcex/HALO_SOUND_LIST_HALO_CHANNEL_globals.h"
#include "../../headers/ws/fio/fioFILE_DISK.h"
#include "../../headers/ws/fio/fio_boundary.h"

extern "C" void osOutputDebugString(const char *fmt, ...); // boundary

// HALO_SOUND_LIST::ParseBin(const char *lst) @ 0x836BEACC
// Fast-path binary tag table: opens "<lst>.bin"; on success, reads a version int (must be 1,
// else this is an old-format file -- strong-assert and bail), then a tag count and, per tag,
// its (nameLength, name[nameLength] NUL-terminated, startIndex, count) triple, appending each
// as a HALO_SOUND_TAG. Debug-asserts the resulting `tags` came in already sorted ascending by
// name (the .bin is expected to have been built that way). Returns false (no strong-assert) if
// the file simply doesn't exist, so the caller falls back to the text .lst parser.
bool HALO_SOUND_LIST::ParseBin(const char *lst)
{
    dsTSTRING<char> binName;
    binName.pBuffer = nullptr;
    binName.UnsafeInit(lst, -1, 0);
    binName.Insert(binName.pBuffer->strLen, ".bin", -1);

    fioFILE_DISK file;
    const char *binNameStr = binName.pBuffer->str;
    if (!file.InitFile(binNameStr, dsFLAGS<FIO_OPEN, int>{ FIO_OPEN_R }, -1))
    {
        osOutputDebugString("Failed to load lst-bin file: %s\n", binNameStr);
        return false;
    }

    int version = 0;
    file.ReadData(&version, 4, 4);
    if (version != 1)
    {
        if (!IGNORE_STRONG_ASSERT)
            STRONG_ASSERT_DUMMY().Crash(
                "!\"Lst-bin: Old program version\"",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1238,
                dsStrongAssertMessage);
        return false;
    }

    int tagCount = 0;
    this->soundsCount = 0;
    file.ReadData(&tagCount, 4, 4);
    if (tagCount > this->tags.allocated)
        this->tags.Realloc(tagCount);

    HALO_SOUND_TAG tag;
    tag.name = dsSTRID();
    tag.startIndex = 0;
    tag.count = 0;
    tag.previousIndex = -1;
    tag.params = nullptr;

    char nameBuf[608];
    for (int i = 0; i < tagCount; ++i)
    {
        unsigned int nameLength = 0;
        file.ReadData(&nameLength, 4, 4);
        if (!IGNORE_STRONG_ASSERT && nameLength >= 0x200)
            STRONG_ASSERT_DUMMY().Crash(
                "nameLength < sizeof(buf)",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1254,
                dsStrongAssertMessage);

        file.ReadData(nameBuf, nameLength, 1);
        if (!IGNORE_STRONG_ASSERT && nameBuf[nameLength - 1] != 0)
            STRONG_ASSERT_DUMMY().Crash(
                "buf[nameLength - 1] == 0",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1257,
                dsStrongAssertMessage);

        tag.name = dsSTRID(nameBuf, false);
        file.ReadData(&tag.startIndex, 4, 4);
        file.ReadData(&tag.count, 4, 4);
        this->tags.PushBack(tag);
        this->soundsCount += tag.count;
    }

    file.Term();

    for (int i = 1; i < this->tags.nElem; ++i)
    {
        if (!IGNORE_STRONG_ASSERT)
        {
            const char *cur = this->tags[i].name.CStr();
            const char *prev = this->tags[i - 1].name.CStr();
            int cmp;
            for (;;)
            {
                unsigned char pc = static_cast<unsigned char>(*prev);
                cmp = pc - static_cast<unsigned char>(*cur);
                if (pc == 0 || cmp != 0)
                    break;
                ++prev;
                ++cur;
            }
            if (cmp >= 0)
                STRONG_ASSERT_DUMMY().Crash(
                    "strcmp(tags[i - 1].name.CStr(), tags[i].name.CStr()) < 0",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1272,
                    dsStrongAssertMessage);
        }
    }

    return true;
}

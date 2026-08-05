#include "../../headers/hcex/HALO_SOUND_LIST.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/hcex/HALO_SOUND_LIST_HALO_CHANNEL_globals.h"
#include "../../headers/ws/fio/fio_boundary.h"

// ds debug array allocator (operator new[] with a source-location cookie). boundary.
void *operator new[](size_t size, const char *file, int line);

// HALO_SOUND_LIST::Preload() @ 0x836C02EC
// isMemory only, and only once (isLoaded guards re-entry): synchronously read the whole .fsb
// file into a freshly-allocated `data` buffer, create its one FMOD::Sound (via CreateSound,
// which reads data/dataLength when isMemory), and block until FMOD finishes opening it.
void HALO_SOUND_LIST::Preload()
{
    if (!this->isLoaded && this->isMemory)
    {
        osFILE_HANDLE_DUMMY *file = osFileOpen(this->fileName.pBuffer->str, 1u);
        if (!IGNORE_STRONG_ASSERT && !file)
            STRONG_ASSERT_DUMMY().Crash(
                "file != 0", "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                1355, empty_string);

        unsigned int size = osFileGetSize(file);
        this->dataLength = size;
        this->data = static_cast<unsigned char *>(::operator new[](
            size, "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 0x54E));
        osFileRead(file, this->data, this->dataLength);
        osFileClose(file);

        this->streams.Resize(1);
        this->streams[0] = this->CreateSound();
        WaitSoundBank(this->streams[0]);
        this->isLoaded = true;
    }
}

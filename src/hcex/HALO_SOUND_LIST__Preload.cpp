/* ?Preload@HALO_SOUND_LIST@@QAAXXZ @0x836C02D0 */
#include "../headers/hcex/HALO_SOUND_LIST_boundary.h"

// 0x836C02D0 — ?Preload@HALO_SOUND_LIST@@QAAXXZ
// isMemory-only: synchronously slurp the whole .fsb into an owned `data` buffer, create its one
// FMOD::Sound and block until it finishes opening. A no-op once isLoaded (or for a streaming list).
void HALO_SOUND_LIST::Preload()
{
    if (!this->isLoaded && this->isMemory) {
        osFILE_HANDLE_DUMMY *file = osFileOpen(this->fileName.pBuffer->str,
                                               dsFLAGS<OS_FILE, int>{OS_FILE_READ});
        if (!IGNORE_STRONG_ASSERT && !file)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("file != 0",
                                       "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                       1355, empty_string);

        this->dataLength = osFileGetSize(file);
        this->data = (unsigned char *)operator new[](
            this->dataLength, "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 0x54E);
        osFileRead(file, this->data, this->dataLength);
        osFileClose(file);

        this->streams.Resize(1);
        this->streams[0] = this->CreateSound();
        WaitSoundBank(this->streams[0]);
        this->isLoaded = true;
    }
}

#include "../headers/hcex/HALO_SOUND_LIST_boundary.h"

// 0x836C4F78 — ?Init@HALO_SOUND_LIST@@QAAXPBD00H@Z
// Stash the diagnostic level name and the .fsb path, load the tag table (try the fast binary
// .lst-bin first, else the text .lst), resolve each tag's HALO_SOUND_PARAMS, then classify the
// list: prepareStreams == -1 marks it a memory-resident bank (loaded lazily on Preload());
// prepareStreams > 0 marks it a streaming list that is "loaded" immediately, with that many
// streaming voices opened up front and waited on.
void HALO_SOUND_LIST::Init(const char *levelName, const char *lst, const char *fsb, int prepareStreams)
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId()) {
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                            "HALO_SOUND_LIST::Init", snd::THREAD_ID, osGetCurThreadId());
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("snd::THREAD_ID == osGetCurThreadId()",
                                       "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                       1155, empty_string);
        return;
    }

    this->levelName = dsTSTRING<char>(levelName);
    this->fileName = dsTSTRING<char>(fsb);

    if (!this->ParseBin(lst))
        this->ParseLst(lst);
    this->InitSetupTagParams();

    this->isMemory = (prepareStreams == -1);
    this->isLoaded = (prepareStreams != -1);

    if (prepareStreams != -1) {
        if (!IGNORE_STRONG_ASSERT && prepareStreams <= 0)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("prepareStreams > 0",
                                       "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                       1173, empty_string);
        this->streams.Resize(prepareStreams);
        this->streamUsed.Resize(prepareStreams, false);
        for (int i = 0; i < prepareStreams; ++i)
            this->streams[i] = this->CreateSound();
        for (int j = 0; j < prepareStreams; ++j)
            WaitSoundBank(this->streams[j]);
    }
}

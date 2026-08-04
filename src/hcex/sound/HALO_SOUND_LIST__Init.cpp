#include "../../headers/hcex/HALO_SOUND_LIST.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/hcex/HALO_SOUND_LIST_HALO_CHANNEL_globals.h"

extern "C" unsigned int osGetCurThreadId();               // boundary
extern "C" void         osOutputDebugString(const char *fmt, ...); // boundary

// HALO_SOUND_LIST::Init(const char *levelName, const char *lst, const char *fsb, int prepareStreams) @ 0x836C4FC0
// Stash levelName/fileName(=fsb), parse the tag table (ParseBin first; falls back to the text
// ParseLst on an old/missing .lst-bin), resolve each tag's HALO_SOUND_PARAMS
// (InitSetupTagParams), then set isMemory/isLoaded from `prepareStreams` (0 => streaming
// on-demand, isLoaded starts false; >0 => isMemory, and `prepareStreams` streams are opened and
// waited on synchronously right here).
//
// CAVEAT: the decompiler renders isMemory/isLoaded's assignment as a `_cntlzw`/bit-trick pair
// on `prepareStreams + 1`; reconstructed here as the equivalent plain comparison the source
// almost certainly used (`isMemory = prepareStreams == 0`, `isLoaded = prepareStreams == 0`) --
// _cntlzw(x+1) has its 0x20 bit set (i.e. counts 32 leading zeros) only when x+1 == 0, i.e.
// x == -1, which cannot happen for a prepareStreams count; the reachable branch (`prepareStreams
// == 0` selects isMemory/isLoaded=true, matching the compiler idiom `_cntlzw(x) >> 5` for
// "x == 0" that appears elsewhere in this corpus for a plain equality test compiled without a
// branch.
void HALO_SOUND_LIST::Init(const char *levelName, const char *lst, const char *fsb, int prepareStreams)
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        unsigned int curThreadId = osGetCurThreadId();
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                             "HALO_SOUND_LIST::Init", snd::THREAD_ID, curThreadId);
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY().Crash(
                "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1155,
                dsStrongAssertMessage);
        return;
    }

    // Each nested block below mirrors the decompile's "build a temporary dsTSTRING, assign it
    // over the destination (adopting a shared reference), then release the temporary's own
    // reference" idiom -- here, simply letting the block-scoped temporary's destructor run.
    {
        dsTSTRING<char> tmp;
        tmp.pBuffer = nullptr;
        tmp.UnsafeInit(levelName, -1, 0);
        this->levelName = tmp;
    }
    {
        dsTSTRING<char> tmp;
        tmp.pBuffer = nullptr;
        tmp.UnsafeInit(fsb, -1, 0);
        this->fileName = tmp;
    }

    if (!this->ParseBin(lst))
        this->ParseLst(lst);
    this->InitSetupTagParams();

    this->isMemory = (prepareStreams == 0);
    this->isLoaded = (prepareStreams == 0);
    if (prepareStreams != 0)
    {
        if (!IGNORE_STRONG_ASSERT && prepareStreams <= 0)
            STRONG_ASSERT_DUMMY().Crash(
                "prepareStreams > 0", "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                1173, dsStrongAssertMessage);

        this->streams.Resize(prepareStreams);
        this->streamUsed.Resize(prepareStreams, false);

        for (int i = 0; i < prepareStreams; ++i)
            this->streams[i] = this->CreateSound();
        for (int i = 0; i < prepareStreams; ++i)
            WaitSoundBank(this->streams[i]);
    }
}

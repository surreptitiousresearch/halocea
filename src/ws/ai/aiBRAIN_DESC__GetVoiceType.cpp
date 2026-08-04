#include "../../headers/ws/ai/aiBRAIN_DESC.h"

// dsSPrintf @ 0x825210C0 — formatted build into a new dsTSTRING (returned by value). boundary.
extern dsTSTRING<char> dsSPrintf(const char *fmt, ...);

// aiBRAIN_DESC::GetVoiceType @ 0x83186AF0
// ?GetVoiceType@aiBRAIN_DESC@@QAAABV?$dsTSTRING@D@@XZ
//
// Return this class's voice-type name. With a single voice (voiceCount <= 1) that is just the base
// voiceType; otherwise a per-call random 1-based suffix is appended ("<voiceType><n>") into a
// process-wide scratch string that is returned by reference. The original keeps `randomizedVoice`
// as a function-local static seeded from the shared empty-string singleton.
const dsTSTRING<char> &aiBRAIN_DESC::GetVoiceType()
{
    static dsTSTRING<char> randomizedVoice;

    if (this->voiceCount <= 1)
        return this->voiceType;

    int suffix = this->voiceRnd.Get() + 1;
    randomizedVoice = dsSPrintf("%s%i", this->voiceType.CStr(), suffix);
    return randomizedVoice;
}

// aiBASS::InternalPlaySound @ 0x83242F88
// ?InternalPlaySound@aiBASS@@MAA_NPBVexSTREAM_DATA@1@AAM@Z
#include "../../headers/ws/ai/aiBASS.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ia/iaSND_PLAY_DATA.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern int IGNORE_STRONG_ASSERT;         // _IGNORE_STRONG_ASSERT
extern unsigned char byte_8200155A;      // shared empty-string / assert-info byte @0x8200155A
extern void _apLog(const char *fmt, ...); // ?_apLog@@YAXPBDZZ — engine logger  boundary

// Actually start `ex`'s backing sound on its owning brain, synchronized. On success, write the
// sound's duration (seconds) into `outTime`, notify the brain (OnExStarted), and report whether the
// duration is positive; on any failure, leave `outTime` = -1 and return false.
bool aiBASS::InternalPlaySound(const exSTREAM_DATA *ex, float &outTime)
{
    if (!IGNORE_STRONG_ASSERT && !ex)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "ex",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_bass.cpp", 776, byte_8200155A);

    outTime = -1.0f;

    aiBRAIN *owner = ex->owner.GetPtr();
    if (!owner)
    {
        _apLog("~BASS~Failed to Play exclamation - no owner");
        return false;
    }

    if (!ex->nameSnd.IsValid())
    {
        _apLog("~BASS~Failed to Play exclamation - invalid name");
        return false;
    }

    iaSND_PLAY_DATA *play = owner->PlaySoundSynced(ex->nameSnd, 1);
    if (!play)
    {
        _apLog("~BASS,Error~Failed to Play exclamation - iaIACTOR::PlaySound() error");
        owner->MuteSound(ex->nameSnd, SMT_NORMAL);
        return false;
    }

    outTime = (float)play->GetTotalTime() * 0.001f;

    dsTSTRING<char> exName(ex->name.CStr());
    owner->OnExStarted(exName, outTime);

    _apLog("~BASS~Play exclamation %s (%.2f)", ex->nameSnd.CStr(), outTime);
    return outTime > 0.000001f;
}

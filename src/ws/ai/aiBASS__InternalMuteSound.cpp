// aiBASS::InternalMuteSound @ 0x83243180
// ?InternalMuteSound@aiBASS@@MAA_NPBVexSTREAM_DATA@1@@Z
#include "../../headers/ws/ai/aiBASS.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern int IGNORE_STRONG_ASSERT;         // _IGNORE_STRONG_ASSERT
extern unsigned char byte_8200155A;      // shared empty-string / assert-info byte @0x8200155A
extern void _apLog(const char *fmt, ...); // ?_apLog@@YAXPBDZZ — engine logger  boundary

// Mute `ex`'s backing sound on its owning brain and fire the "cancelled" event. Returns false when
// there is no owner or the sound name is invalid (nothing to do), true once muted.
bool aiBASS::InternalMuteSound(const exSTREAM_DATA *ex)
{
    if (!IGNORE_STRONG_ASSERT && !ex)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "ex",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_bass.cpp", 816, byte_8200155A);

    aiBRAIN *owner = ex->owner.GetPtr();
    if (!owner)
        return false;

    if (!ex->nameSnd.IsValid())
        return false;

    _apLog("~BASS~Mute exclamation %s", ex->nameSnd.CStr());
    owner->MuteSound(ex->nameSnd, SMT_NORMAL);

    dsTSTRING<char> exName(ex->name.CStr());
    owner->OnExCancelled(exName);
    return true;
}

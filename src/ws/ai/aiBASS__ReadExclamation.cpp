// aiBASS::ReadExclamation @ 0x83244868
// ?ReadExclamation@aiBASS@@IAAXABVpsSECTION@@PBD@Z
#include "../../headers/ws/ai/aiBASS.h"
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"

// Parse one exclamation category `name` out of section `psRead` and append it to `exclamations`.
// Cooldown defaults come from the BASS-wide values; overlap policy is parsed from a string key.
// The section handle arrives as a psSECTION but is accessed through the value-semantic
// psSECTION_INTERFACE view (same 4-byte layout), matching the decompile's cast.
void aiBASS::ReadExclamation(const psSECTION &psRead, const char *name)
{
    const psSECTION_INTERFACE &r = static_cast<const psSECTION_INTERFACE &>(psRead);

    exDATA ex;
    ex.name            = dsSTRID(name, 0);
    ex.priority        = r.GetFloat("priority", 0.0f, 0);
    ex.aiCoolDown      = r.GetFloat("aiCoolDown", this->aiCoolDown, 0);
    ex.exCoolDown      = r.GetFloat("exCoolDown", this->exCoolDown, 0);
    ex.exMaxCount      = r.GetInt("exMaxCount", -1, 0);
    ex.isAcceptForDead = r.GetBool("isAcceptForDead", 0, 0) == 1;

    dsTSTRING<char> overlapText = r.GetStr("overlapResolving", dsTSTRING<char>(), 0);
    ex.overlap = ParseOverlap(&overlapText, exOVERLAP_SKIP);

    exclamations.PushBack(ex);
}

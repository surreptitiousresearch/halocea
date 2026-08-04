// aiBASS::ParsePS @ 0x83244FF8
// ?ParsePS@aiBASS@@MAA_NVpsSECTION@@AAV?$dsTSTRING@D@@@Z
#include "../../headers/ws/ai/aiBASS.h"
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"

// Read the BASS-wide tuning fields from section `psRead`, then walk its "exclamations" sub-section
// and read each named exclamation category. Fails (with a hint) when no "exclamations" section is
// present. `psRead` is a by-value psSECTION handle accessed via the psSECTION_INTERFACE view.
bool aiBASS::ParsePS(psSECTION psRead, dsTSTRING<char> &hintErr)
{
    psSECTION_INTERFACE &r = static_cast<psSECTION_INTERFACE &>(psRead);

    hintErr = dsTSTRING<char>(); // start with an empty hint

    if (r.IsKeyExist("exclamationPrefix", 0))
        exclamationPrefix = r.GetStr("exclamationPrefix", dsTSTRING<char>(), 0);
    if (r.IsKeyExist("exclamationPostfix", 0))
        exclamationPostfix = r.GetStr("exclamationPostfix", dsTSTRING<char>(), 0);

    aiCoolDown       = r.GetFloat("aiCoolDown", aiCoolDown, 0);
    exCoolDown       = r.GetFloat("exCoolDown", exCoolDown, 0);
    minSimSoundDelay = r.GetFloat("minSimSoundDelay", minSimSoundDelay, 0);
    maxDistance      = r.GetFloat("maxDistance", maxDistance, 0);
    distanceDelta    = r.GetFloat("distanceDelta", distanceDelta, 0);
    maxSounds        = r.GetInt("maxSounds", maxSounds, 0);

    if (!r.IsKeyExist("exclamations", 0))
    {
        hintErr = "no 'exclamations' section";
        return false;
    }

    psSECTION exSection = r.GetSection("exclamations", 0);
    psSECTION_INTERFACE &exView = static_cast<psSECTION_INTERFACE &>(exSection);

    int nKey = exView.GetNKey();
    for (int i = 0; i < nKey; ++i)
    {
        dsSTRID     keyName = exView.GetKeyName(i);
        const char *keyStr  = keyName.CStr();
        psSECTION   child   = exView.GetSection(keyStr, 0);
        if (child.pData)
            ReadExclamation(child, keyStr);
    }
    return true;
}

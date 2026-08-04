#include "../../headers/ws/ai/aiBRAIN_DESC_parse_boundary.h"
#include "../../headers/ws/ai/aiGOAL_BASE.h"       // aiGOAL_BASE::ParseSpline
#include "../../headers/ws/ps/psITERATOR.h"
#include "../../headers/ws/ds/ds_boundary.h"        // dsDATA_TYPE_STORAGE<T>::dataType
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include <string.h>

// aiBRAIN_DESC::ReadVoices(const psSECTION&) @ 0x83186F30
// Read the voice block: voiceType/voiceCount (with a random-exclusive picker for count>1),
// voiceTeam, and per-sound pitch-vs-velocity curves from the "sounds.sounds_list" sub-section.
void aiBRAIN_DESC::ReadVoices(const psSECTION &psReadS)
{
    const psSECTION_INTERFACE &psRead = static_cast<const psSECTION_INTERFACE &>(psReadS);

    if (psRead.IsKeyExist("voiceType", 0))
    {
        dsTSTRING<char> def;
        def.UnsafeInitEmpty();
        voiceType = psRead.GetStr("voiceType", def, 0);
    }

    if (strcmp(voiceType.pBuffer->str, "mute") != 0)
    {
        voiceCount = psRead.GetInt("voiceCount", voiceCount, 0);
        if (voiceCount > 1)
            voiceRnd.Init(voiceCount);
        if (voiceCount < 1)
        {
            voiceType = dsTSTRING<char>("mute");
            voiceCount = 0;
        }
    }
    else
    {
        voiceCount = 0;
    }

    if (psRead.IsKeyExist("voiceTeam", 0))
    {
        dsTSTRING<char> def;
        def.UnsafeInitEmpty();
        dsTSTRING<char> s = psRead.GetStr("voiceTeam", def, 0);
        if (s.pBuffer->strLen != 0 && strcmp(s.pBuffer->str, "none") != 0)
            voiceTeam = dsSTRID(s, false);
        else
            voiceTeam = dsSTRID();
    }

    // Per-sound pitch curves: iterate every record of "sounds.sounds_list".
    psSECTION sounds = psRead.GetSection("sounds", 0);
    psSECTION soundsList = static_cast<psSECTION_INTERFACE &>(sounds).GetSection("sounds_list", 0);

    psITERATOR it(soundsList, PS_TYPEID_INVALID, 0);
    while (!it.IsDone())
    {
        // (Original computes the record's key name here; the result is unused.)
        psSECTION keySec;
        it.GetKeySection(&keySec);
        static_cast<psSECTION_INTERFACE &>(keySec).GetKeyName(it.GetKeyIdx());

        dsDATA val = it.GetValue();
        if (val.type == dsDATA_TYPE_STORAGE<psSECTION>::dataType)
        {
            psSECTION sec;
            if (psDataToSection(&val, &sec))
            {
                dsTSTRING<char> curve;
                curve.UnsafeInitEmpty();
                if (static_cast<psSECTION_INTERFACE &>(sec).GetDSString("pitchVelCurve", &curve, 0))
                    aiGOAL_BASE::ParseSpline(&curve, &curveSndPitch);
            }
        }
        it.Next();
    }
}

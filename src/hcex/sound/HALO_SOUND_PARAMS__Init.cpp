// ?Init@HALO_SOUND_PARAMS@@QAAXABVpsSECTION@@@Z -- 0x836BFEC0
#include "../../headers/hcex/HALO_SOUND_PARAMS.h"
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"
#include "../../headers/ws/ps/psITERATOR.h"

// psSECTION_INTERFACE adds no fields over psSECTION (it's the same 4-byte handle, just with a
// richer accessor surface), so a `const psSECTION&` can be viewed as one directly.
void HALO_SOUND_PARAMS::Init(const psSECTION &ps)
{
    const psSECTION_INTERFACE &section = static_cast<const psSECTION_INTERFACE &>(ps);

    tag = section.GetStrid("name", dsSTRID(), 0);

    volume = section.GetFloat("volume", 1.0f, 0);
    volume2D = section.GetFloat("volume2D", 1.0f, 0);

    distGtTag = section.GetStrid("onDistFar.name", dsSTRID(), 0);
    distGt = section.GetFloat("onDistFar.value", -1.0f, 0);
    state.val = (distGt < 0.0f) ? (state.val & ~HALO_SOUND_HAS_DIST_GT) : (state.val | HALO_SOUND_HAS_DIST_GT);

    distLtTag = section.GetStrid("onDistNear.name", dsSTRID(), 0);
    distLt = section.GetFloat("onDistNear.value", -1.0f, 0);
    state.val = (distLt < 0.0f) ? (state.val & ~HALO_SOUND_HAS_DIST_LT) : (state.val | HALO_SOUND_HAS_DIST_LT);

    dist2D = section.GetFloat("panLevel.dist2D", -1.0f, 0);
    dist3D = section.GetFloat("panLevel.dist3D", -1.0f, 0);
    if (dist2D > dist3D)
        dist2D = dist3D;
    bool hasPanLevel = (dist2D >= 0.0f) && (dist3D >= 0.0f);
    state.val = hasPanLevel ? (state.val | HALO_SOUND_UPDATE_PANLEVEL) : (state.val & ~HALO_SOUND_UPDATE_PANLEVEL);

    mouthTag = section.GetStrid("mouthName", dsSTRID(), 0);
    mouthMinDelay = section.GetInt("mouthMinDelay", 0, 0);
    state.val = mouthTag.IsEmpty() ? (state.val & ~HALO_SOUND_HAS_MOUTH_SOUND) : (state.val | HALO_SOUND_HAS_MOUTH_SOUND);

    bool playToEnd = section.GetBool("isPlayToEnd", 0, 0) != 0;
    state.val = playToEnd ? (state.val | HALO_SOUND_PLAY_TO_END) : (state.val & ~HALO_SOUND_PLAY_TO_END);

    psSECTION replaceSection = section.GetSection("replace", 0);
    if (replaceSection.pData)
    {
        for (psITERATOR it(replaceSection, PS_TYPEID_SECTION, 0); !it.IsDone(); it.Next())
        {
            psSECTION entrySection;
            it.GetSection(&entrySection);
            const psSECTION_INTERFACE &entry = static_cast<const psSECTION_INTERFACE &>(entrySection);

            REPLACE_INFO entryInfo;
            entryInfo.source = entry.GetStrid("source", dsSTRID(), 0);
            entryInfo.tag = entry.GetStrid("tag", dsSTRID(), 0);

            if (entryInfo.source.IsValid() && entryInfo.tag.IsValid())
                replace.PushBack(entryInfo);
        }
        replace.Realloc(replace.nElem);
    }
}

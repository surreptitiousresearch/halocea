#include "../../headers/ws/ai/aiAPPROACH.h"
#include "../../headers/ws/ps/psSECTION.h"
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"
#include "../../headers/ws/ps/psARRAY.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"
#include <string.h>

// _apLog — engine formatted log sink. boundary.
extern void _apLog(const char *fmt, ...);

// aiAPPROACH::ParsePS(const psSECTION &) @ 0x8329AD60
// ?ParsePS@aiAPPROACH@@IAA_NABVpsSECTION@@@Z
//
// Parse the "radius", "notice" (player1/player2/ally list) and "isActOnce" keys of an approach
// section into `radius` and the notice/act-once state bits. The returned flag mirrors the binary:
// it is set (non-zero) when a notice-array element could not be read or the notice set ended up
// empty; SetByName treats a non-zero return as "accepted" (see aiAPPROACH__SetByName.cpp).
bool aiAPPROACH::ParsePS(const psSECTION &psRaw)
{
    const psSECTION_INTERFACE &ps = static_cast<const psSECTION_INTERFACE &>(psRaw);
    bool flagged = false;

    radius = ps.GetFloat("radius", 2.0f, 0);

    unsigned int val = state.val;
    if (ps.IsKeyExist("notice", 0))
    {
        state.val = val & ~0x3000u; // clear all notice bits

        psARRAY notice = ps.GetArray("notice", 0);
        for (int i = 0; i < notice.Length(); ++i)
        {
            dsDATA elem = notice.GetElement(i);
            dsTSTRING<char> who;
            if (elem.GetValue<dsTSTRING<char> >(who, DSD_CONV_RETRIEVE))
            {
                if (strcmp(who.CStr(), "player1") == 0)
                    state.val |= AI_APST_NOTICE_PLAYER1;
                else if (strcmp(who.CStr(), "player2") == 0)
                    state.val |= AI_APST_NOTICE_PLAYER2;
                else if (strcmp(who.CStr(), "ally") == 0)
                    state.val |= AI_APST_NOTICE_ALLY;
            }
            else
            {
                _apLog("~Error,AI~Error parsing APPROACH: cannot parse \"notice\" array");
                flagged = true;
            }
        }

        // No notice target resolved from the array -> flag it.
        if ((state.val & AI_APST_NOTICE_ALLY) == 0)
        {
            _apLog("~Error,AI~Error parsing APPROACH: empty \"notice\" array");
            flagged = true;
        }
    }
    else
    {
        state.val = val | AI_APST_NOTICE_ALLY; // default: notice everyone
    }

    if (ps.GetBool("isActOnce", 0, 0))
        state.val |= AI_APST_ACT_ONCE;
    else
        state.val &= ~AI_APST_ACT_ONCE;
    state.val &= ~AI_APST_ACTED;

    return flagged;
}

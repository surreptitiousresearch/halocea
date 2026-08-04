#include "../../headers/ws/ai/aiGRASS.h"
#include "../../headers/ws/nav/navWP.h"
#include "../../headers/ws/nav/navPENMAN.h"

// 0x8265BB00 (?m3dDist@@YAMPIBUm3dV@@0@Z) — Euclidean distance between two points. boundary.
extern float m3dDist(const m3dV *a, const m3dV *b);
// 0x... (?_m3dCheckValid@@YAHPBUm3dV@@@Z) — non-NaN/inf validity check. boundary.
extern int _m3dCheckValid(const m3dV *v);
// 0x84169778 (_scale) — global tuning weight for the inverse-distance grenade penalty term. boundary.
extern float scale;

// Strong-assert plumbing (mirrors the cseries STRONG_ASSERT macro). boundary.
extern int IGNORE_STRONG_ASSERT;
extern unsigned char STRONG_ASSERT_INFO_STUB; // byte_8200155A
struct STRONG_ASSERT_DUMMY {
    static void Crash(void *thisPtr, const char *tag, const char *file, int line,
                      unsigned char &info);
};

// aiGRASS::CalcPenaltyForBuildPath @ 0x83183650
// ?CalcPenaltyForBuildPath@aiGRASS@@QBAMPAVnavWP@@@Z  (const)
//
// Accumulate a nav-path penalty at waypoint `wp` from every currently tracked grenade: each
// grenade contributes navPENMAN::CalcGrenadePenalty(dist) plus an inverse-distance term.
float aiGRASS::CalcPenaltyForBuildPath(navWP *wp) const
{
    if (!this->grenades.head)
        return 0.0f;

    m3dV posWP;
    wp->GetEffectivePos(&posWP);
    if (!IGNORE_STRONG_ASSERT && !_m3dCheckValid(&posWP))
        STRONG_ASSERT_DUMMY::Crash(nullptr, "_m3dCheckValid(&posWP)",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_grass.cpp", 319,
            STRONG_ASSERT_INFO_STUB);

    float penalty = 0.0f;
    for (ds::LIST<GRENADE_INFO>::DATA *node = this->grenades.head; node; node = node->next)
    {
        float dist = m3dDist(&posWP, &node->val.posLastKnown) + 0.000001f;
        penalty += navPENMAN::CalcGrenadePenalty(dist) + (scale / dist);
    }
    return penalty;
}

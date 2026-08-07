#pragma once
// ws-engine ai08/wpn: muzzle aim-check mode selecting which tip query a weapon strike uses.
// DB-verified values (types_enum_values aimCHECK_TYPE, type_ordinal 3374; types.size = 4),
// corroborated verbatim by headers_ref/aimCHECK_TYPE.h.
// Observed at the call sites (a8_weapon.cpp), and consistent with the names: aimBY_BODY/aimBY_FSM
// -> body-relative tip (GetPosDirTipFromBody and a dir-up tangent projection in IsAimedByPosAndDir);
// aimBY_WPN/aimALWAYS_AIMED -> weapon-shooter tip (GetPosDirTipFromWpn); aimUNDEF -> "resolve from
// the controller descriptor" (GetAimCheck).
enum aimCHECK_TYPE : int {
    aimBY_WPN       = 0,
    aimBY_BODY      = 1,
    aimBY_FSM       = 2,
    aimALWAYS_AIMED = 3,
    aimUNDEF        = 4,
};

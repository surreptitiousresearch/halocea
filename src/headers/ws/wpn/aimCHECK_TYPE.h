#pragma once
// ws-engine ai08/wpn: muzzle aim-check mode selecting which tip query a weapon strike uses.
// Observed at the call sites (a8_weapon.cpp): value 1 or 2 -> body-relative tip (GetPosDirTipFromBody
// and a dir-up tangent projection in IsAimedByPosAndDir); 0 or 3 -> weapon-shooter tip
// (GetPosDirTipFromWpn); 4 -> "resolve from the controller descriptor" (GetAimCheck). The concrete
// enumerator names live in the wpn re-source frontier; kept as an opaque enum so no values are
// invented.
enum aimCHECK_TYPE : int;

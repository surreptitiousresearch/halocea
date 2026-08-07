#pragma once
#include "fsmCB_PARAM_BASE.h"
// ws-engine fsm: the parameter block a weapon-list FSM passes to its subscribers when a weapon is
// added/activated/deactivated/deleted (aiWEAPON::OnWpn* handlers). Only the leading weapon pointer
// is exercised by the ai08 weapon module (disasm a8_weapon.cpp @0x83294DD8 reads +0x00).

struct wpnWEAPON; // wpn — a single equipped weapon (fwd)

// DB-verified layout (types_members fsmWPN_LIST_PARAM_WEAPON, types size 8): fsmCB_PARAM_BASE
// <base>@0 (empty, size 0), wpn@0 (wpnWEAPON *), isChangeInProgress@4 (bool).
struct fsmWPN_LIST_PARAM_WEAPON : fsmCB_PARAM_BASE {
    wpnWEAPON *wpn;                // 0x00 the weapon this notification concerns
    bool       isChangeInProgress; // 0x04 a weapon swap is still in flight
};

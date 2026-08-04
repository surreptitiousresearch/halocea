#pragma once
// ws-engine fsm: the parameter block a weapon-list FSM passes to its subscribers when a weapon is
// added/activated/deactivated/deleted (aiWEAPON::OnWpn* handlers). Only the leading weapon pointer
// is exercised by the ai08 weapon module (disasm a8_weapon.cpp @0x83294DD8 reads +0x00); the rest of
// the block belongs to the fsm re-source frontier and stays opaque.

struct wpnWEAPON; // wpn — a single equipped weapon (fwd)

struct fsmWPN_LIST_PARAM_WEAPON {
    wpnWEAPON *wpn; // 0x00 the weapon this notification concerns
};

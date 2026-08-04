#pragma once
#include "../ds/dsVECTOR.h"
// ws-engine prop: an AI-usable weapon controller — the prop object through which a brain drives an
// equipped weapon. DB-verified layout (types_members propWPN_AI_USABLE): propUSABLE base@0 (268),
// __weapon@0x10C, isActive@0x110, listSml@0x114 (dsVECTOR<int,8>) — size 296. The propUSABLE base
// stays byte-exact opaque (next frontier).

struct wpnWEAPON;  // wpn — a single equipped weapon (fwd)
struct wpnS_DESC;  // wpn — weapon strike descriptor (fwd)
enum wpnSTRIKE_STATUS { STRIKE_INACTIVE = 0, STRIKE_PERFORMED = 1, STRIKE_IN_PAUSE = 2 };

struct propWPN_AI_USABLE {
    unsigned char   _pad0_propUSABLE[268]; // 0x000 propUSABLE base (opaque)
    wpnWEAPON      *__weapon;              // 0x10C DEFAULT_CTOR<wpnWEAPON*>
    bool            isActive;              // 0x110
    unsigned char   _pad111[3];
    dsVECTOR<int,8> listSml;               // 0x114 sml-state list

    // ?weapon@propWPN_AI_USABLE@@QBAPBVwpnWEAPON@@XZ — the equipped weapon (null if none).
    // boundary — body external to this batch.
    const wpnWEAPON *weapon() const;
    // ?weapon@propWPN_AI_USABLE@@QAAPAVwpnWEAPON@@XZ — non-const overload. boundary.
    wpnWEAPON *weapon();

    // ?GetStrikeStatus@propWPN_AI_USABLE@@... — current strike status for descriptor `desc`.
    // boundary — body external to this batch.
    wpnSTRIKE_STATUS GetStrikeStatus(const wpnS_DESC *desc) const;

    // ?PerformStrike@propWPN_AI_USABLE@@... — begin performing the strike described by `desc`.
    // boundary — body external to this batch.
    void PerformStrike(const wpnS_DESC *desc);

    // 0x831D0940 (?Reload@propWPN_AI_USABLE@@QAAXXZ) — consume a clip on the equipped weapon (the
    // ammo-side of a full reload). Used by aiWEAPON::Reload.  boundary — body external to this batch.
    void Reload();
};

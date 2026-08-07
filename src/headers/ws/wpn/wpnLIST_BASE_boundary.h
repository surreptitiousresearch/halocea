#pragma once
#include "../ctrl/ctrlPROP_ID.h"
#include "wpnSHOOTER.h"
// ws-engine wpn subsystem — boundary surface reached by aiWATCHER::IsBigGun and the ai08 weapon
// module. The full layouts belong to the wpn re-source frontier; the fields the AI touches are
// typed here (DB-verified: propFSM base@0/156, shooter@0x9C, spreadScale@0xF8) and the remainder
// stays byte-exact opaque.

struct atkDSP_INFO; // wpn/atk — attack-dispatch info (fwd)
struct iaIACTOR;    // ia — interactive actor owning this weapon (fwd)

// A single equipped weapon.
struct wpnWEAPON {
    unsigned char _pad0[0x0C];        // 0x00 propFSM base head (opaque)
    iaIACTOR     *pOwner;             // 0x0C owning actor (DB-derived: a8_weapon.cpp @0x83294DFC)
    unsigned char _pad10[0x9C - 0x10];// 0x10 propFSM base tail (opaque)
    wpn::SHOOTER  shooter;            // 0x9C firing sub-object (28 bytes)
    // DB tail (types_members wpnWEAPON): entUser@0xB8 ds::WEAK_PTR<entENTITY>, target@0xBC
    // gsGEOM_ANCHOR, spreadScale@0xF8 float, offTipCheckShooting@0xFC, forceTipShooting@0xFD,
    // wpnListName@0x100 dsSTRID. Kept opaque here; padded to DB-verified total size 260.
    unsigned char _padTail[260 - (0x9C + 28)]; // 0xB8..0x103

    // ?GetType@wpnWEAPON@@QBA?AUctrlPROP_ID@@XZ — the weapon's type property id (by value).
    ctrlPROP_ID GetType() const;

    // ?GetAttackInfo@wpnWEAPON@@... — fill `info` for attack `atkID`; returns whether it applies.
    // boundary — body external to this batch.
    bool GetAttackInfo(int atkID, atkDSP_INFO *info);

    // ?SendCmd@wpnWEAPON@@... — post FSM command `cmd` (an interned dsSTRID, e.g. cmdWPN_RELOAD) to
    // this weapon with the parameter block `par`. Used by aiWEAPON::ReloadInstantly.  boundary.
    void SendCmd(const struct dsSTRID *cmd, struct fsmWPN_ATK_INFO *par);
};

// A unit's weapon list / inventory.
// DB layout for the wpn frontier (types_members wpnLIST_BASE, size 160): propSYNCABLE base@0/44,
// wpnSlots@44/20 dsVECTOR<dsPAIR<dsSTRID,wpnWEAPON*>,8>, eventHandler@64/92 fsmCB_HANDLER<dsSTRID>,
// activeSlotName@156/4 dsSTRID. Deliberately modelled opaque: every use in the tree is a
// wpnLIST_BASE* (aiWATCHER::GetWeaponList, aiWEAPON::wpnList@0x1220, aiWATCHER_PLAYER::wpnCtrl@0xAC),
// so no TU depends on the size or layout.
struct wpnLIST_BASE {
    // ?GetWeaponCur@wpnLIST_BASE@@QBAPAVwpnWEAPON@@XZ — the currently selected weapon (null if none).
    wpnWEAPON *GetWeaponCur() const;
};

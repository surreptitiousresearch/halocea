#pragma once
// ws-engine wpn: the firing sub-object of a weapon — owns the barrel pool, the shooter descriptor
// and the current projectile-speed override. DB-verified layout (types_members wpn::SHOOTER):
// barrels@0 (dsVECTOR<wpn::BARREL,8>,20), pDesc@0x14, customPjlSpeed@0x18 — size 28.

#include "../ds/dsSMART_PTR.h" // dsSMART_PTR / _dsSP_OWNER_PLC_REFCOUNT (GetAtkAmmoClip return)
#include "../ds/dsVECTOR.h"

struct m3dV;          // ws m3d — 3-vector (fwd)
struct wpnAMMO_CLIP;  // wpn — a weapon's ammo clip (fwd; ptr in the returned smart-ptr)
struct dsREF_COUNT;   // ds — intrusive ref-count policy tag (fwd)

namespace wpn {

struct SHOOTER_DESC; // wpn — shooter descriptor (fwd)
struct BARREL;       // wpn — one barrel (tips + ammo clip); pointer-elements only here (fwd)

struct SHOOTER {
    dsVECTOR<BARREL, 8> barrels; // 0x00 barrel pool (DB: dsVECTOR<wpn::BARREL,8>)
    SHOOTER_DESC *pDesc;         // 0x14 shooter descriptor
    float         customPjlSpeed; // 0x18 projectile-speed override (<0 == none)

    // ?IsAtkValid@SHOOTER@wpn@@QBA_NH@Z — true when `atkID` names an attack this shooter supports.
    // boundary — body external to this batch.
    bool IsAtkValid(int atkID) const;

    // ?IsEmpty@SHOOTER@wpn@@QBA_NXZ — true when the shooter currently holds no ammo / cannot fire.
    // Used by aiWEAPON::ReloadInstantly.  boundary — body external to this batch.
    bool IsEmpty() const;

    // ?GetOrgDirTip@SHOOTER@wpn@@... — write the muzzle-tip origin and firing direction into
    // `pos`/`dir`. The trailing flags select barrel-averaging / tip-vs-origin behaviour.
    // boundary — body external to this batch.
    void GetOrgDirTip(m3dV *pos, m3dV *dir, int barrelSel, int useTip) const;

    // 0x82FFE3B8 (?GetAtkAmmoClip@SHOOTER@wpn@@QBA?BV?$dsSMART_PTR@UwpnAMMO_CLIP@@...@@@H@Z) — const:
    // the ammo clip backing attack `atkID` (an owning smart-ptr; null pointee when none). Used by
    // aiRELOADER::IsLoaded.  boundary — body external to this batch.
    dsSMART_PTR<wpnAMMO_CLIP, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > GetAtkAmmoClip(int atkID) const;
};

} // namespace wpn

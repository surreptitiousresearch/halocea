#pragma once
#include "../ap/apSTATE_T.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsSMART_PTR.h"
// ws-engine prop: a fireable / health component (damage, health, death state).
// DB-verified layout (types_members propFIREABLE / propBASE base chain) — size 0x144. Modelled as a
// flat boundary that pins the named offsets the ai08 aiPROP_FIREABLE batch actually reaches; the
// propSYNCABLE/mp::MSG_IFACE base internals and the trailing damage-cache members (delayedDmg,
// hitStrength, dmgs, ...) are byte-accurate opaque placeholders (their bodies are a later frontier).
//   propBASE base: pOwner@0x0C, spDesc@0x10, sslObject@0x14 (opaque).
//   propFIREABLE:  maxHealth@0x2C, state@0x30, healthCur@0x34, isRevitalizeMode@0x38,
//                  objFireable@0x3C, delayedDmg@0x50 ... lastClearTime@0x140.

struct iaIACTOR;         // ws/ia — interactive actor (owning unit)          boundary (fwd/ptr)
struct objOBJ;           // ws/obj — game object                             boundary (fwd/ptr)
struct propBASE_DESC;    // ws/prop — property descriptor base               boundary (fwd/ptr)
struct dmgTYPE;          // ws/dmg — damage type descriptor                  boundary (fwd/ptr)
struct dmgDAMAGE;        // ws/dmg — damage event                            boundary (fwd/ptr)
struct gsGEOM_ANCHOR;    // ws/gs — geometry anchor                          boundary (fwd/ptr)
struct dsREF_COUNT;      // ws/ds — intrusive ref-count                      boundary (fwd)
template<class T> struct dsTSTRING;

struct propFIREABLE {
    unsigned char             _pad00[0x0C];               // 0x00 propBASE vtbl + WEAK_PTR + pPropOwner
    iaIACTOR                 *pOwner;                      // 0x0C owning interactive actor
    dsSMART_PTR<propBASE_DESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > spDesc; // 0x10 owned descriptor
    unsigned char             _pad14[0x2C - 0x14];        // 0x14 sslObject + typeId (opaque)
    float                     maxHealth;                  // 0x2C
    apSTATE_T<unsigned long>  state;                      // 0x30 state bits (bit1=dead, bit2=dying)
    float                     healthCur;                  // 0x34 current health
    bool                      isRevitalizeMode;           // 0x38
    unsigned char             _pad39[3];
    dsVECTOR<objOBJ *, 8>     objFireable;                // 0x3C per-object fireable filter list
    unsigned char             _pad50[0x144 - 0x50];       // 0x50 delayedDmg/hitStrength/dmgs/... (opaque)

    // --- virtual/method surface the ai08 batch dispatches (declared as plain members; the gate does
    //     not model vtable layout). Bodies external to this batch unless a src/<...>.cpp exists.  ---
    bool IsRevitalizeMode() const;                        // revitalize-on-death enabled
    void Revitalize();                                    // restore from a downed state (0x82FD3450)
    int  Hp() const;                                      // ?Hp@propFIREABLE@@UBAHXZ (0x82FD1AE8) current hp
    bool IsFriendlyFire() const;                          // ?IsFriendlyFire@propFIREABLE@@UBA_NXZ (0x829D1E48)
    bool ProcessINIT();                                   // ?ProcessINIT@propFIREABLE@@UAA_NXZ (0x82FDADC8)
    bool IsInvulnerable() const;
    bool IsGodMode() const;
    void AcceptDamage(const dmgDAMAGE *pDmg);             // base accept-damage
    void Die(const dsTSTRING<char> &nameSeq);             // base death (kill-word sequence)
    void SendDamage(dmgDAMAGE *pDmg);                     // base broadcast-damage
    void ShootThrough(const dmgDAMAGE *pDmg);             // pass-through when already dead
    bool HasHitDeathSys();                                // owner has a hit/death FSM system
};

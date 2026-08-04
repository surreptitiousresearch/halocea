#pragma once
#include "../m3d/m3dV.h"
#include "../ds/dsTSTRING.h" // SetPar key

// ws-engine ai08 — base of a brain's perception module (aiMIND::perc pointee). Only the query
// surface reached by the batched aiMIND methods is declared; the full layout/bodies are the next
// frontier.  boundary.
//
// NOTE: aiWATCHER_BRAIN_boundaries.h carries a separate, consumer-local boundary subset of
// aiPERC_BASE (NoticeGrenade/NoticeGrenadeTerm only). These two boundary views must not be included
// in the same TU (same established pattern as aiENEMY_BASE). This standalone header is the one used
// by the aiMIND-module consumers.

struct entENTITY; // ent subsystem — game entity  boundary (ptr)

struct aiPERC_BASE {
    void *__vftable; // 0x00 aiPERC_BASE_vtbl*

    void NoticeGrenade(entENTITY *grenade);     // vtbl — a grenade was noticed  boundary
    void NoticeGrenadeTerm(entENTITY *grenade); // vtbl — a tracked grenade ended boundary

    // ?NoticeFlyObject@aiPERC_BASE@@UAAXABUm3dV@@@Z — virtual: a physics object passed near `pos`.
    // Used by aiMIND::NoticeFlyObject.  boundary.
    void NoticeFlyObject(const m3dV &pos);

    // ?UpdateTimers@aiPERC_BASE@@UAAXM@Z — virtual: advance this module's timers by `dt` seconds.
    // Used by aiMIND::UpdateTimers.  boundary.
    void UpdateTimers(float dt);

    // ?SetPar@aiPERC_BASE@@UAA_NABV?$dsTSTRING@D@@M@Z — virtual: apply a named-tunable value; true if
    // consumed. Used by aiMIND::SetPar. boundary. (folded in from aiWATCHER_BRAIN_boundaries subset)
    bool SetPar(const dsTSTRING<char> &key, float val);
    // ?ProcessInit@aiPERC_BASE@@UAAXXZ — virtual: per-spawn init hook. Used by aiMIND::ProcessINITLive.
    void ProcessInit();
    // ?NoticeWounded@aiPERC_BASE@@UAAXPAVentENTITY@@M@Z — virtual: the unit took `amount` damage from
    // `shooter` (may be null). Used by aiMIND::NoticeWounded. boundary.
    void NoticeWounded(entENTITY *shooter, float amount);

    // ?ProcessFrame@aiPERC_BASE@@... — virtual: per-frame perception update (advance by `dt`
    // seconds). Used by aiMIND::UpdateAllPerception.  boundary.
    void ProcessFrame(float dt);

    // vtbl — perception "float" query (here: the enemy-damaged fraction). Used by
    // aiBRAIN::GetFloatSSL (GETF_ENEMY_DAMAGED).  boundary — body external to this batch.
    float GetFloat() const;
};

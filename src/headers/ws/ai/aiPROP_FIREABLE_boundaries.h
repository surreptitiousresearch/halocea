#pragma once
#include "../ds/dsVECTOR.h"
#include "../nav/navWP_ID_WEIGHT.h"
// Boundary declarations for the ai08 aiPROP_FIREABLE batch (a8_prop_fireable.cpp). These wrap the
// deep/mangled accesses the decompiler emitted as raw owner-array indexing (this->pOwner[2].snd...,
// spDesc.pointee[N]...) and the sub-systems that lie past this wave's bounded 2-level descent
// (ssl event dispatch, navGPH weight-graph selection, fsm body events, the cdtINFO/random-impulse
// self-damage build). Each helper carries the original decompiler expression it stands in for and is
// the NEXT re-source frontier — no data layout is asserted here.

struct iaIACTOR;
struct objOBJ;
struct navWP;
struct wpWP;
struct navSYS;
struct navMANAGER;
struct aiBRAIN;
struct aiBRAIN_IFACE;
struct dmgDAMAGE;
struct gsGEOM_ANCHOR;
struct animINST;
struct dsSTRID;
struct aiPROP_FIREABLE;
typedef struct m3dV m3dV;

// The planner singleton (aiPLANNER *aiPlanner) and its owned nav tables.  boundary.
extern struct aiPLANNER *aiPlanner;

// --- IsAcceptDamage ---------------------------------------------------------------------------
bool    aiDEBUG_IsGodMode();                                  // aiDEBUG::IsGodMode()
objOBJ *gsGEOM_ANCHOR_GetObj(const gsGEOM_ANCHOR *hitInfo);   // gsGEOM_ANCHOR::GetObj(hitInfo)

// --- CalcDamageFromPlayer ---------------------------------------------------------------------
bool aifireable_FriendlyFireAI();                             // dbg_fireable_friendly_fire_ai.value != 0
bool aifireable_OwnerImmuneToPlayerFF(iaIACTOR *owner);       // LOBYTE(owner->spDesc.pointee[42].refCount)

// --- CalcDamageToApply ------------------------------------------------------------------------
bool           aifireable_CanBleed(iaIACTOR *owner);          // MDL_PROP_ST_CAN_BLEED bit-array test on owner
aiBRAIN_IFACE *aifireable_ShooterBrain(const dmgDAMAGE *pDmg);// pDmg->params.entShooter.pHandle->pPtr as brain-iface
bool           aifireable_IsBrainDesc(aiBRAIN_IFACE *b);      // b typeid IsDerivedFrom aiBRAIN_DESC::TYPE_ID
float          aifireable_AiDamageScale(aiBRAIN_IFACE *b, const dmgDAMAGE *pDmg); // brain SSL distance-scale
bool           aifireable_IsPlayer(aiBRAIN_IFACE *b);         // gssIsPlayer(b)

// --- Die(dsTSTRING) / ProcessTowerMan self-damage & impulse -----------------------------------
float aibrain_TowerManStrength(iaIACTOR *owner);              // owner->GetTowerManStrength()
animINST *aibrain_Inst(aiBRAIN *owner);                       // owner->pInst
// Fill dmg->params.orgDmg/dirDmg with the randomized topple origin/direction and build the self-hit
// gsGEOM_ANCHOR (cdtINFO::Init + gsGEOM_ANCHOR::Init) used by aiPROP_FIREABLE::Die(dsTSTRING).
void  aifireable_BuildSelfDamageAnchor(dmgDAMAGE *dmg, aiBRAIN *owner);
float m3dLengthVector_2(const m3dV *v);                       // m3dLengthVector_2

// fsm/nav event name-ids (interned dsSTRID, returned by value in the DB — modelled by const-ptr).
const dsSTRID *fsmRAGDOLL_EVENT_start();
const dsSTRID *fsmPHYS_EVENT_apply_speed();
// wpWP payload queries.
bool  wpWP_IsTwrMan(wpWP *wp);
bool  wpWP_HaveChilds(wpWP *wp);
float wpWP_GetTwrManStrgth(wpWP *wp);
void  navWP_GetDir(navWP *wp, m3dV *out);                     // navWP::GetDir(&out)
void  m3dNormalize(m3dV *v);                                  // m3dNormalize
extern const float M3D_EPSILON_2_633;                         // m3d near-zero length-squared epsilon

// --- FindTowerManWPbyNS -----------------------------------------------------------------------
navSYS *aifireable_GoalNavSystem(aiBRAIN *pBrain);            // pBrain->mind.goal.pointee->GetNavSystem()
// Build the tower-man weighting functors and collect the best waypoints within maxDist from the
// nav-system weight graph (navGPH::GetBestWPsByWeightFunc). Deeper than this wave's descent.
void aifireable_GetBestTowerManCandidates(navSYS *ns, aiBRAIN *pBrain, float maxDist,
                                          dsVECTOR<navWP_ID_WEIGHT, 8> *out);

// --- FindTowerManWPwoNS -----------------------------------------------------------------------
bool                  aifireable_BrainSkipTowerMan(aiBRAIN *pBrain); // BYTE1(spDesc.pointee[40].pBrand)
dsVECTOR<short, 8>   *aifireable_TowerManWPList(navMANAGER *nm);     // nm->towermanWPList

// --- SendDamage / AcceptDamage / Die(dmgDAMAGE) : owner body / ssl / stat dispatch -------------
// owner body message-target vtbl call at +92 (this->pOwner[2].snd.playList.numLoopSounds).
void aifireable_SendBodyHitEvent(iaIACTOR *owner, dmgDAMAGE *pDmg, bool wounding);
// owner container pre-damage hook (pOwner[2].propContainer.pPropOwner->Hide(pDmg)).
void aifireable_OwnerPreDamage(iaIACTOR *owner, const dmgDAMAGE *pDmg);
// shooting-precision-stat hit record (gmShootingPrecisionStat->eventHandler ActivateCB spsON_HIT).
void aifireable_RecordShootingPrecision(aiPROP_FIREABLE *self, const dmgDAMAGE *pDmg);
// aiMIND::NoticeWounded on the owner mind ((aiMIND*)&pOwner[2].snd.playList.list).
void aifireable_NoticeWounded(iaIACTOR *owner, const dmgDAMAGE *pDmg);
// gsIs<aiBRAIN>(owner) strong-assert guard.
bool aifireable_OwnerIsBrain(iaIACTOR *owner);
// debug global damage multiplier (dbg_dmg.value).
float aifireable_DbgDmgScale();
// aiPROP_FIREABLE::Die(dmgDAMAGE) branch bodies (ssl OnBleed vs OnDeath/OnAIKill + kill notify +
// tower-man topple + body die fsm). Both traverse ssl/dsDATA/propContainer past this descent.
void aifireable_HandleBleed(aiPROP_FIREABLE *self);
void aifireable_HandleDeath(aiPROP_FIREABLE *self, const dmgDAMAGE *pDmg);

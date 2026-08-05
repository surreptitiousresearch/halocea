#pragma once
// Boundary declarations for the aiDOM_SPAWN method re-source batch: engine globals and free
// functions the methods reference. All layouts stay canonical (see the included headers); only
// extern-linkage symbols are declared here. These are the next-frontier edges.

#include "../m3d/m3dV.h"
#include "../m3d/m3dRND_GEN.h"
#include "../ds/dsTSTRING.h"
#include "../msg/msgRES.h"

struct aiPLANNER;
struct msgADDR;
struct aiDOM_SPAWN;
struct rendDRIVER;
struct scnSCENE;
struct scnDOMAIN;
struct dsSTATE_MGR;
struct dsDATA;
struct entENTITY;
struct entCREATE_DATA;
struct psSECTION;

// --- engine globals ---
extern m3dRND_GEN   m3dRndGen;          // shared RNG
extern aiPLANNER   *aiPlanner;          // global AI planner (may be null)
extern float        gsElapsedTime;      // per-frame dt
extern rendDRIVER  *rendDrv;            // render driver
extern scnSCENE    *gsScenePtr;         // active scene
extern dsSTATE_MGR *gStateMgr;          // networked state manager
extern unsigned int gsAppState2;        // app-state flags (secondary word)
extern int          ent_idx;            // monotonically increasing entity spawn counter
extern int          IGNORE_STRONG_ASSERT;


// AI debug cvars — each wraps a `.value` field (bool-ish). Modeled minimally.
struct aiDBG_BOOL_CVAR { int value; /* + cvar bookkeeping — boundary */ };
extern aiDBG_BOOL_CVAR dbg_isOffSpawn;           // 0x843257C8
extern aiDBG_BOOL_CVAR dbg_isRenderSpawnDomains; // 0x843257D8
extern aiDBG_BOOL_CVAR dbg_isRenderSpawnPoints;  // 0x843257E8

// --- free functions ---
// player enumeration
int        gsGetPlayerCount();
entENTITY *gsGetPlayer(int index);
// entity factory (returns spawned entity, or null on failure)
entENTITY *entCreate(scnSCENE *scene, const char *className, const char *templateName,
                     entCREATE_DATA *cd, void *parent);
// vector NaN/inf validation guard (no-op in release)
void _m3dCheckValid(const m3dV *v);
void _m3dCheckValid(float f);
// vector globals + helpers used by GetSpawnPosDir (m3d subsystem — boundary)
extern const m3dV m3dVUnitZ;   // +Z basis vector
extern const m3dV m3dVZero;    // zero vector
float m3dDist(const m3dV *a, const m3dV *b);   // Euclidean distance
void  m3dNormalize(m3dV *v);                    // normalise in place
// engine log
extern void _apLog(const char *fmt, ...);
// string formatter into a dsTSTRING. DB return type is non-const dsTSTRING<char>*
// (?dsSPrintf@@YAPAV?$dsTSTRING@D@@PAV1@PBDZZ); must match hcex_ds_boundary.h to avoid
// a return-type-only overload conflict when both are pulled into one TU.
dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);
// tracked heap free (dsSMART/dsTSTRING intrusive teardown). C linkage: dlFree is an
// unmangled C export — must match the extern "C" canonical decls (hcex_ds_boundary.h).
extern "C" void dlFree(void *ptr);
// dominant-spawn geometry helper — vertical offset applied to spawned instances
namespace dipGEN_SPAWN { float GetShiftOverFloor(); }

// gsIs<T> RTTI predicate (declared per-instantiation at the call site)
template<class T> int gsIs(const void *obj);

// SpawnSingle gates on the networked "IsHost" state. The underlying machinery is a cached
// dsSTATE_MGR::RegisterState("IsHost") + GetState + dsDATA::GetValue<bool> lookup against
// gStateMgr; wrapped here as a boundary helper (state-manager subsystem is the next frontier).
bool aiSpawn_IsNetHost();

// gsDOM_SPAWN base helper used by InitCreateData to seed the create-data name/domain.
void gsDOM_SPAWN_InitCreateData(aiDOM_SPAWN *self, entCREATE_DATA *cd, psSECTION *ps,
                                dsTSTRING<char> *nameCls);

// gsDOMAIN::ProcessMsg base dispatch (default message handling). aiDOM_SPAWN::ProcessMsg tail-calls
// it after intercepting its own messages. Called on the aiDOM_SPAWN via its gsDOMAIN subobject.
msgRES gsDOMAIN_ProcessMsg(aiDOM_SPAWN *self, int msg, void *pInfo, msgADDR *pSend);

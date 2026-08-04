#pragma once
#include "../ia/iaDESC.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsSTRID.h"
#include "../ap/apSTATE_T.h"
#include "../m3d/m3dV.h"
#include "../sml/smlSTATE_ID.h"
#include "../DEFAULT_CTOR.h"
#include "../farm/farmCOND_DATA.h"
// ws-engine ent: per-class entity descriptor (template data shared by all instances of an
// entity class) — the ent-subsystem counterpart to entENTITY.
// Full DB-verified layout (types_members entDESC) — size 440.

struct entDESC_SFX_INFO; // boundary — per-class sfx binding (vector element)
struct psSECTION;        // ps/psSECTION.h — section handle (by-value ParsePS param)

struct entDESC : iaDESC {
    // Numeric sml state ids for this entity class's SHOW / HIDE states (resolved at class init).
    // Read by entENTITY::implShow/implHide to drive sml::STATE transitions. boundary.
    int GetState_SHOW() const;
    int GetState_HIDE() const;

    DEFAULT_CTOR<bool> isSslClassInited;      // 0x0A4
    unsigned char      _padA5[3];             // 0x0A5 db-verified padding
    int sslFuncId_Hide;                       // 0x0A8
    int sslFuncId_Show;                       // 0x0AC
    int sslFuncId_HideObj;                    // 0x0B0
    int sslFuncId_ShowObj;                    // 0x0B4
    int sslFuncId_ShowPhys;                   // 0x0B8
    int sslFuncId_HidePhys;                   // 0x0BC
    int sslFuncId_Shootable;                  // 0x0C0
    int sslFuncId_Aimable;                    // 0x0C4
    int sslFuncId_SetFollow;                  // 0x0C8
    int sslFuncId_SetFollowCurOffset;         // 0x0CC
    int sslFuncId_SetFollowToEntity;          // 0x0D0
    int sslFuncId_EndFollow;                  // 0x0D4
    int sslFuncId_Transport;                  // 0x0D8
    int sslFuncId_TransportPosDir;            // 0x0DC
    int sslFuncId_TransportToEntity;          // 0x0E0
    int sslFuncId_IsSpawnedFrom;              // 0x0E4
    int sslFuncId_InitLightSet;               // 0x0E8
    int sslFuncId_SetSfxParam;                // 0x0EC
    int sslFuncId_StartSfx;                   // 0x0F0
    int sslFuncId_StopSfx;                    // 0x0F4
    int sslFuncId_StopSfxAll;                 // 0x0F8
    int sslFuncId_RestartSfx;                 // 0x0FC
    int sslFuncId_PlaySfx;                    // 0x100
    int sslFuncId_PlaySfxAll;                 // 0x104
    int sslFuncId_EnableDomEvent;             // 0x108
    int sslFuncId_HaveAffix;                  // 0x10C
    int sslFuncId_SetScale;                   // 0x110
    int sslFuncId_Scale;                      // 0x114
    int sslFuncId_GetScale;                   // 0x118
    int sslFuncId_SetLOD;                     // 0x11C
    int sslFuncId_MPSendEvent;                // 0x120
    int sslFuncId_SetFarmCounter;             // 0x124
    int sslFuncId_GetFarmCounter;             // 0x128
    int sslFuncId_ChangeFarmCounter;          // 0x12C
    int sslFuncId_Colorise;                   // 0x130
    int sslFuncId_GetClassName;               // 0x134
    int sslFuncId_SetMaterial;                // 0x138
    int sslFuncId_IsNearCamera;               // 0x13C
    int sslFuncId_HasTag;                     // 0x140
    int sslFuncId_SetOrientationByNormal;     // 0x144
    int sslFuncId_SetIntensity;               // 0x148
    smlSTATE_ID __smlState_SHOW;              // 0x14C
    smlSTATE_ID __smlState_HIDE;              // 0x150
    dsVECTOR<entDESC_SFX_INFO, 8> infoSfx;    // 0x154
    dsTSTRING<char> nameTplDefault;           // 0x168 default name-template string
    float scaleEffect;                        // 0x16C
    m3dV  scaleInst;                          // 0x170
    farmCOND_DATA dataFarm;                   // 0x17C (16B)
    apSTATE_T<unsigned long> maskSuspend;     // 0x18C
    apSTATE_T<unsigned long> maskSuspendExcl; // 0x190
    dsTSTRING<char> nameConstrActor;          // 0x194
    dsTSTRING<char> nameConstrObj;            // 0x198
    dsVECTOR<dsSTRID, 8> tags;                // 0x19C
    float timeToFreeze;                       // 0x1B0
    int   dbgID;                              // 0x1B4

    // 0x8269F9F0 — pointer to the character data of the default name-template string.
    // Mangle QBAPADXZ: const method, returns non-const char* (PAD, not PBD).
    char *GetNameTplDefault() const;

    // 0x826AB218 (?ParsePS@entDESC@@UAAXVpsSECTION@@ABV?$dsTSTRING@D@@@Z) — virtual: parse the shared
    // entity-class descriptor fields from `ps`, tagging errors with `hintErr`. boundary.
    void ParsePS(psSECTION ps, const dsTSTRING<char> &hintErr);

    // ?TYPE_ID@entDESC@@2VdsTYPE_ID@@A — static RTTI id for the entity-descriptor class; used by
    // aiPLANNER nav dispatch to test whether a message sender is an entity. boundary.
    static dsTYPE_ID TYPE_ID;

    // 0x826AC0D8 (?InitSslClass@entDESC@@UAAHABV?$dsTSTRING@D@@@Z) — virtual: base SSL-class init;
    // chained by derived InitSslClass overrides (e.g. aiBRAIN_DESC). boundary.
    int InitSslClass(const dsTSTRING<char> &hintErr);
}; // 440 bytes

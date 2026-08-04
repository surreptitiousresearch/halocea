#pragma once
// ws-engine AI spawn domain. DB-verified layout (types_members aiDOM_SPAWN) — size 456 (0x1C8).
// Inheritance chain: aiDOM_SPAWN : aiDOM_SPW_IFACE : gsDOM_SPAWN : gsDOMAIN : (iaIACTOR@0,
//   scnDOMAIN@0xC8, SSL_SYNC_OBJECT@0x124). aiDOM_SPW_IFACE / gsDOM_SPAWN add no new subobject
//   size over their 0x188-byte prefix, so aiDOM_SPAWN's own fields start at 0x188.
//
// The base graph (iaIACTOR/scnDOMAIN/SSL_SYNC_OBJECT/gsDOMAIN/gsDOM_SPAWN) is a boundary here:
// only the individual base fields the aiDOM_SPAWN methods actually touch are named, at their
// DB-verified byte offsets, with opaque padding between them. This preserves binary layout
// without pulling the full base header graph into this frontier.
//
//   spDesc      @0x54  (iaIACTOR)   dsSMART_PTR<dscDESC,...>  — descriptor (indexed as dscDESC[])
//   nmbLocks    @0xB0  (iaIACTOR)   int
//   scnDomain   @0xC8  (scnDOMAIN)  opaque 0x5C region (RenderDom takes &scnDOMAIN)
//   stateDom    @0x140 (gsDOMAIN)   apSTATE_T<unsigned long>  (.state bit0 = active, bit10 = ...)
//   subDomList  @0x144 (gsDOMAIN)   dsVECTOR<scnDOMAIN*,8>
//   nSpawned    @0x178 (gsDOM_SPAWN) int

#include "../ds/dsSMART_PTR.h"
#include "../dsc/dscDESC.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "../ap/apSTATE_T.h"
#include "../m3d/m3dV.h"
#include "../m3d/m3dRND_EXCLUSIVE.h"
#include "../dip/dipPOINT.h"
#include "../msg/msgRES.h"

struct scnDOMAIN;           // gsDOMAIN scnDOMAIN base subobject       boundary (region only)
struct aiDOM_SPAWN_DESC;    // dscDESC-derived descriptor                boundary
struct aiDOM_SPAWN_vtbl;    // vtable                                    boundary
struct entCREATE_DATA;      // ent create-data                           full header via .cpp
struct psSECTION;           // ps parse section                          full header via .cpp
struct aiBRAIN_IFACE;       // messaging peer                            boundary
struct msgADDR;             // message address (ProcessMsg 4th arg base)  boundary

struct aiDOM_SPAWN {
    aiDOM_SPAWN_vtbl *__vftable;                                    // 0x00
    unsigned char     _pad04[0x54 - 0x04];                         // 0x04
    dsSMART_PTR<dscDESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > spDesc; // 0x54
    unsigned char     _pad58[0x5C - 0x58];                         // 0x58
    apSTATE_T<unsigned long> stateSslEv;                          // 0x5C (iaIACTOR)
    unsigned char     _pad60[0xB0 - 0x60];                         // 0x60
    int               nmbLocks;                                    // 0xB0
    unsigned char     _padB4[0xC8 - 0xB4];                         // 0xB4
    unsigned char     scnDomain[0x124 - 0xC8];                     // 0xC8 scnDOMAIN subobject region
    unsigned char     _pad124[0x140 - 0x124];                      // 0x124 SSL_SYNC_OBJECT region
    apSTATE_T<unsigned long> stateDom;                             // 0x140
    dsVECTOR<scnDOMAIN *, 8> subDomList;                           // 0x144
    unsigned char     _pad158[0x178 - 0x158];                      // 0x158 pDomSys/pointsInside/timeTo*Cur
    int               nSpawned;                                    // 0x178
    unsigned char     _pad17C[0x188 - 0x17C];                      // 0x17C dir (m3dV)
    int               nKill;                                       // 0x188
    int               isBunchInProgress;                           // 0x18C
    int               nKillComplete;                               // 0x190
    float             timeAfterKillCur;                            // 0x194
    float             timeAfterSpawn;                              // 0x198
    dsVECTOR<dipPOINT, 8> pointsList;                              // 0x19C
    m3dRND_EXCLUSIVE  rxClass;                                     // 0x1B0
    m3dRND_EXCLUSIVE  rxAffix;                                     // 0x1B8
    m3dRND_EXCLUSIVE  rxBhv;                                       // 0x1C0

    // --- this batch (aiDOM_SPAWN.cpp files) ---
    void   DoSpawn();                    // 0x83264748  AAA (private)
    int    IsActiveSpawn();              // 0x83264800  AAA (private)
    int    GetNameBHV(char *buf, unsigned int size); // 0x83264858 UAA (virtual)
    void   Update();                     // 0x83264AF0  MAA (virtual)
    void   InitCreateData(entCREATE_DATA &cd, psSECTION ps, dsTSTRING<char> nameCls); // 0x83264B80 MAA
    void   SpawnSingle();                // 0x83264F70  EAA (virtual thunk target)
    void   DebugRender(int msg, unsigned int color); // 0x832653A0 AAA
    void   CalcWeightRandom();           // 0x832654B0  AAA
    void   AddWeights(m3dV &center, float scale);    // 0x832655A0 AAA
    msgRES ProcessMsg(int msg, void *pInfo, msgADDR *pSend); // 0x832656B8 MAA (virtual)
    void   AddWeightsByExisting();       // 0x83265718  AAA
    void   AddWeightsByPlayers();        // 0x832658A8  AAA
    void   CalcWeightEven();             // 0x83265A08  AAA

    // --- this batch (aiDOM_SPAWN.cpp files, wave 2) ---
    int    IsReadySpawn();               // 0x83264638  AAA
    void   AccountDead(aiBRAIN_IFACE *pSend); // 0x83264540 AAA
    void   MarkCompleted();              // 0x832641F8  AAA (private) — set completed state + log
    int    Init();                       // 0x832642A0  MAA (virtual)
    // Load previously-authored spawn points (returns true when a saved set exists); on miss Init
    // regenerates them via GeneratePointsList. Bodies external to this batch.  boundary.
    int    LoadDomainPoints();
    void   GeneratePointsList();
    void   SelectSpawnPos(m3dV *pos);    // 0x83265AA0  AAA — pick lowest-weight spawn point
    void   GetSpawnPosDir(m3dV *pos, m3dV *dir); // 0x83265B68 UAA — pos + dir toward nearest player

    dsTSTRING<char> GetName() const;     // inherited (iaIACTOR/entENTITY) — display name
};

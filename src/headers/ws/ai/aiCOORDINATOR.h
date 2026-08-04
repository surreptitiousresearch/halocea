#pragma once
#include "aiDEBUG_boundaries.h" // aiDBGSHAPE_VERT, navASSOC_INFO, dsVECTOR<>, dbgVAR_IMPL<>
#include "../ds/dsVECTOR.h"     // groups / groupSslClassList vectors
#include "../ds/MAP.h"          // resultsUsed hash map
#include "../ds/dsTSTRING.h"    // aiCOORD_GROUP_SSL_CLASS_DESC::groupClassName
#include "../ds/dsSTRID.h"      // aiCOORD_GROUP_SSL_CLASS_DESC::groupType / dsSTRID params
#include "../ssl/sslCLASS_REF.h" // aiCOORD_GROUP_SSL_CLASS_DESC::sslGrpClass / by-value returns
#include "../m3d/m2dV.h"        // BuildGroupWPSetCurveMesh curve points
#include "../m3d/m3dMATR.h"     // BuildGroupWPSetCurveMesh start matrix
#include "../m3d/m3dCOLOR.h"    // curve gradient con-var color values
#include "../wb/dbgVAR_boundary.h" // dbgVAR_IMPL<> curve con-vars

// ws-engine ai08: the coordinator sub-object embedded in aiPLANNER (aiPLANNER::coordinator @0x184,
// DB-verified size 3264 / 0xCC0). It owns the set of aiCOORD_GROUPs, the per-brain used-WP result
// map, the group-ssl-class registry, and the group-debug draw buffers.
// Layout is DB-verified (types_members aiCOORDINATOR); the previously-opaque 0x04..0x84 prefix is
// now fully typed to support the group create/join/leave and penalty methods.

struct aiBRAIN;                    // ai08 per-unit AI brain (full definition in aiBRAIN.h)
struct aiCOORD_GROUP;              // ai08 coordinated group (aiCOORD_GROUP_boundary.h)
struct m3dV;                       // 3d vector (full definition in m3d/m3dV.h)
struct navWFUNC_SQAS;              // nav weighting function (nav/navWFUNC_SQAS_boundary.h)
struct navSYS;                     // nav-system (nav/navSYS.h) — used by pointer in debug-mesh builders

struct aiCOORDINATOR {
    // Registry entry: an ssl group class bound to a (name, type) pair. DB-verified layout
    // (types_members aiCOORDINATOR::aiCOORD_GROUP_SSL_CLASS_DESC): sslGrpClass@0, groupClassName@4,
    // groupType@8 — size 12.
    struct aiCOORD_GROUP_SSL_CLASS_DESC {
        sslCLASS_REF     sslGrpClass;    // 0x00
        dsTSTRING<char>  groupClassName; // 0x04
        dsSTRID          groupType;      // 0x08
    };

    // --- dispatched virtuals (DB aiCOORDINATOR_vtbl slots) ---
    virtual void           NoticeWounded(aiBRAIN *pBrain, float amount);      // vtbl 0x14 REVERSED
    virtual bool           IsGroupLeader(aiBRAIN *pBrain);                    // vtbl 0x28 REVERSED
    virtual const aiCOORD_GROUP *GetGroup(aiBRAIN *pBrain);                   // vtbl 0x2C boundary
    virtual aiCOORD_GROUP *GetGroup_2(aiBRAIN *pBrain);                       // vtbl 0x30 boundary
    virtual aiCOORD_GROUP *GetGroup_3(const dsTSTRING<char> *grpName);        // vtbl 0x34 boundary
    virtual void           DebugRender(aiBRAIN *watchee);                     // vtbl 0x48 REVERSED
    virtual int            GetGroupMembersCount(aiBRAIN *pBrain);             // vtbl 0x5C REVERSED

    // --- per-frame / lifetime virtuals (this batch) ---
    virtual void  ProcessFrame(float dt);                                    // REVERSED 0x83237E70
    virtual void  DropAll();                                                 // REVERSED 0x83237908
    virtual void  MarkWPAsUnused(aiBRAIN *pBrain);                           // REVERSED 0x832379A0

    // --- a8_coord ssl-registration / group-name batch (this batch) ---
    virtual void            ProcessInit();                                   // REVERSED 0x83234208
    virtual bool            RegisterGroupClass(const dsTSTRING<char> &name,
                                               const dsSTRID &classId);      // boundary 0x832372D0 (next frontier)
    virtual const dsTSTRING<char> &GetGroupName(aiBRAIN *pBrain);            // REVERSED 0x832344E0
    virtual dsTSTRING<char> GetGroupTypeName(aiBRAIN *pBrain);              // REVERSED 0x83234590 (sret)
    virtual void            NoticeRemoveOtherGrpMember(aiCOORD_GROUP *grp,
                                                       aiBRAIN *pBrain);     // REVERSED 0x83235420 (const in binary)

    // --- group create / join / leave / query virtuals (this batch) ---
    virtual int   GetGroupIdx(aiBRAIN *pBrain) const;                        // REVERSED 0x83235540
    virtual bool  JoinToGroup(aiBRAIN *pBrain, const dsTSTRING<char> &groupName,
                              const dsSTRID &groupType, dsTSTRING<char> &hint); // REVERSED 0x83235CD8
    virtual short GetResultWP(aiBRAIN *pBrain) const;                        // REVERSED 0x83235DA8
    virtual bool  CreateGroup(aiBRAIN *pBrain, dsTSTRING<char> groupName,
                              const dsSTRID &groupType, bool canJoinToExist,
                              dsTSTRING<char> &hint);                        // REVERSED 0x832364D0
    virtual void  LeaveGroup(aiBRAIN *pBrain);                               // REVERSED 0x83236928
    virtual float GetPenaltyCloseWP(short wpid, const m3dV &wpPos,
                                    const navWFUNC_SQAS &wfun,
                                    aiBRAIN *pBrainSelf) const;              // REVERSED 0x83236990

    // --- protected group helpers (this batch) ---
protected:
    void         NoticeRemoveOtherGroup(aiCOORD_GROUP *grp) const;           // REVERSED 0x83235620
    bool         JoinToGroup(aiBRAIN *pBrain, const dsTSTRING<char> &groupName,
                             const dsSTRID &groupType, const sslCLASS_REF &grpClass,
                             dsTSTRING<char> &hint);                         // REVERSED 0x83235738
    sslCLASS_REF GetGroupSslClass(const dsTSTRING<char> &groupClassName);    // REVERSED 0x83235908
    sslCLASS_REF GetGroupSslClassByType(const dsSTRID &groupType);           // REVERSED 0x83235A00
    void         FillDebugRenderAssoc(const dsVECTOR<navASSOC_INFO, 8> &dbgAssocInfo); // REVERSED 0x83235AE8
    float        CalcPenaltyFromUsedWPs(short wpid, const m3dV &wpPos,
                                        const navWFUNC_SQAS &wfun,
                                        aiBRAIN *pBrainSelf) const;          // REVERSED 0x83235E18
    void         RemoveDeletedGroups();                                      // REVERSED 0x83236038
    void         AddGroupSslClass(const sslCLASS_REF &sslGrpClass,
                                  const dsTSTRING<char> &groupClassName,
                                  const dsSTRID &groupType);                 // REVERSED 0x83236A20

    // --- debug-mesh builders for the currently-watched group (this batch) ---
    void  UpdateDebugRenderInfo();                                           // REVERSED 0x83237C20
    void  BuildGroupWPSetCurveMesh(const dsVECTOR<m2dV, 8> &curvePoints,
                                   const m3dMATR &startMatr);                // REVERSED 0x83236BA8
    void  BuildGroupFrontsMesh(const navSYS *dbgUsedNS,
                               const dsVECTOR<dsTSTRING<char>, 8> &dbgUsedFrontList,
                               const dsVECTOR<dsTSTRING<char>, 8> &dbgExclFrontList); // REVERSED 0x83237A20
    void  AddGroupFrontsMesh(const navSYS *dbgUsedNS, const dsTSTRING<char> &frontName,
                             unsigned long color, dsVECTOR<aiDBGSHAPE_VERT, 8> &vBuf,
                             dsVECTOR<unsigned short, 8> &iBuf);             // REVERSED 0x83237008

public:
    // --- data layout (DB types_members aiCOORDINATOR) ---
    dsVECTOR<aiCOORD_GROUP *, 8> groups;                     // 0x04 live groups
    int          groupNextID;                                // 0x18 auto-group name counter
    ds::MAP<aiBRAIN *, short, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> resultsUsed; // 0x1C brain->wpid
    dsVECTOR<aiCOORD_GROUP_SSL_CLASS_DESC, 8> groupSslClassList; // 0x70 ssl group-class registry
    short         dbgGroupWpSetWatchee[100];                 // 0x84
    int           dbgGroupWpSetWatcheeCount;                 // 0x14C
    dsVECTOR<aiDBGSHAPE_VERT, 8> dbgGroupWPSetCurveVBuf;     // 0x150
    dsVECTOR<unsigned short, 8>  dbgGroupWPSetCurveIBuf;     // 0x164
    dsVECTOR<aiDBGSHAPE_VERT, 8> dbgGroupExcludersVBuf;      // 0x178
    dsVECTOR<unsigned short, 8>  dbgGroupExcludersIBuf;      // 0x18C
    dsVECTOR<aiDBGSHAPE_VERT, 8> dbgGroupFrontsVBuf;         // 0x1A0
    dsVECTOR<unsigned short, 8>  dbgGroupFrontsIBuf;         // 0x1B4
    navASSOC_INFO dbgGroupAssocWatchee[100];                 // 0x1C8
    int           dbgGroupAssocWatcheeCount;                 // 0xCB8
    aiBRAIN      *currentWatchee;                            // 0xCBC
};

// ai08 group-debug con-var toggles read by aiCOORDINATOR::DebugRender. boundary.
extern dbgVAR_IMPL<bool, 1>  dbg_isCoordGroupCurve_Enable;
extern dbgVAR_IMPL<float, 3> dbg_isCoordGroupCurve_Alpha;
extern dbgVAR_IMPL<bool, 1>  dbg_isCoordGroupExcl_Enable;
extern dbgVAR_IMPL<float, 3> dbg_isCoordGroupExcl_Alpha;
extern dbgVAR_IMPL<bool, 1>  dbg_isCoordGroupFront_Enable;
extern dbgVAR_IMPL<float, 3> dbg_isCoordGroupFront_Alpha;
extern dbgVAR_IMPL<bool, 1>  dbg_isCoordGroupInfo;
extern dbgVAR_IMPL<bool, 1>  dbg_isCoordGroupBrainInfo;
extern dbgVAR_IMPL<bool, 1>  dbg_isCoordGroupAssocInfo;

// ai08 group-curve-mesh gradient con-vars read by aiCOORDINATOR::BuildGroupWPSetCurveMesh.
// DB template tags: m3dCOLOR->9, float->3, int->2. boundary.
extern dbgVAR_IMPL<m3dCOLOR, 9> dbg_isCoordGroupCurve_ColorMin;
extern dbgVAR_IMPL<m3dCOLOR, 9> dbg_isCoordGroupCurve_ColorMax;
extern dbgVAR_IMPL<float, 3>    dbg_isCoordGroupCurve_ValueMin;
extern dbgVAR_IMPL<float, 3>    dbg_isCoordGroupCurve_ValueMax;
extern dbgVAR_IMPL<float, 3>    dbg_isCoordGroupCurve_VScale;
extern dbgVAR_IMPL<int, 2>      dbg_isCoordGroupCurve_NSeg;

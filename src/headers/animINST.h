#pragma once
/* animINST — a ws-engine animated scene instance. Boundary type (ws-engine C++); full 536-byte
 * (0x218) layout DB-verified against types_members animINST. Pointer members carry
 * forward-declared typed pointees. */

#include <stdint.h>
#include "animTPL.h"
#include "ws/m3d/m3dV.h"
#include "ws/m3d/m3dMATR.h"
#include "ws/m3d/m3dOBB.h"
#include "ws/m3d/m3dBOX.h"
#include "ws/ps/psSECTION.h"
#include "ws/ds/dsAFFIX_STRING.h"
#include "ws/ds/dsVECTOR.h"
#include "ws/ds/dsSMART_PTR.h"
#include "ws/ds/dsREF_COUNT.h"
#include "ws/ap/apSTATE_T.h"
#include "ws/os/osLOCK.h"
#include "ws/rend/rendDYN_LIGHT_STATE.h"
#include "ws/anim/animCULL_INFO.h"

typedef struct entENTITY entENTITY;
struct animMNG;
struct animLOD;
struct animTRACKER;
struct scnSCENE;
struct cdtSKEL;
struct physRAGDOLL_BASE;
struct physSYSTEM_BASE;
struct rendLIGHT_SET;
struct objOBJ;
struct animCREATE_DATA;

/* Object-modifier cache state — 4 bytes. DB-verified enum
 * (types_enum_values animINST::<unnamed_type_objMOD_CACHE_STATE>); the IDA/mangler name is kept
 * verbatim per the _LARGE_INTEGER.h convention. */
enum objMOD_CACHE_STATE_e
{
    OBJMODCACHE_INVALID  = 0,
    OBJMODCACHE_CACHE_OK = 1,
};

typedef struct animINST
{
    unsigned int  sign;      /* 0x000 */
    int           state;     /* 0x004 */
    int           state2;    /* 0x008 */
    entENTITY    *pEnt;      /* 0x00C */
    uint8_t stateVis;/* 0x010 */
    unsigned char _pad11[3]; /* 0x011 */
    m3dV          prevPosVis;/* 0x014 */
    int           stateVld;  /* 0x020 validity-flag word */
    int           id;        /* 0x024 */
    char         *name;      /* 0x028 */
    char         *nameClass; /* 0x02C */
    char         *nameTpl;   /* 0x030 */
    animINST     *prev;      /* 0x034 */
    animINST     *next;      /* 0x038 */
    m3dMATR       matrL2W;   /* 0x03C local-to-world transform */
    m3dV          scale;     /* 0x07C per-instance scale */
    animMNG      *pAnimMng;  /* 0x088 */
    objOBJ       *pObjRoot;  /* 0x08C */
    dsSMART_PTR<rendLIGHT_SET, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > pLightSet; /* 0x090 */
    rendDYN_LIGHT_STATE dynLightsState; /* 0x094 */
    dsAFFIX_STRING affixes;  /* 0x0AC */
    physRAGDOLL_BASE *pPhysChar;   /* 0x0B0 */
    physSYSTEM_BASE  *pPhysSystem; /* 0x0B4 */
    unsigned int  cdtLayer;  /* 0x0B8 */
    char          idAnimCdt; /* 0x0BC */
    unsigned char _padBD[3]; /* 0x0BD */
    float         obbExpansion; /* 0x0C0 */
    m3dOBB        obb;       /* 0x0C4 oriented bounding box */
    m3dBOX        bbox;      /* 0x100 axis-aligned bounding box */
    int           lastFameVisible; /* 0x118 */
    int           nObjRend;  /* 0x11C */
    objOBJ      **ppObjListRend;   /* 0x120 */
    int64_t      *pObjCamMask;     /* 0x124 */
    bool          isObjRendVis;    /* 0x128 */
    unsigned char _pad129[3];      /* 0x129 */
    m3dV          prevPos;   /* 0x12C */
    m3dV          entSpeed;  /* 0x138 */
    cdtSKEL      *pSkel;     /* 0x144 collision skeleton */
    objOBJ       *pObj;      /* 0x148 model object list head */
    int           nObj;      /* 0x14C */
    objOBJ      **ppObjList; /* 0x150 */
    float         fVisDistFactor;  /* 0x154 */
    psSECTION     ps;        /* 0x158 */
    animTPL      *pTpl;      /* 0x15C template asset (name at pTpl->name) */
    animLOD      *pLOD;      /* 0x160 */
    scnSCENE     *pScn;      /* 0x164 owning scene */
    void         *pAuxData;  /* 0x168 */
    osLOCK        perInstLock; /* 0x16C */
    int16_t       idxLoaded; /* 0x1A0 */
    unsigned char _pad1A2[2];/* 0x1A2 */
    animTRACKER  *followTracker;   /* 0x1A4 */
    int           nCallsClacLTM;   /* 0x1A8 */
    int           nCallsApplyAnim; /* 0x1AC */
    int           isCheckNeed;     /* 0x1B0 */
    int           isCheckReady;    /* 0x1B4 */
    uint16_t nLTMLogged;   /* 0x1B8 */
    uint16_t nAnimLogged;  /* 0x1BA */
    animCULL_INFO cullInfo;  /* 0x1BC */
    enum objMOD_CACHE_STATE_e objMOD_CACHE_STATE; /* 0x1F0 */
    dsVECTOR<short, 8> objectsWithModifiersCache; /* 0x1F4 */
    apSTATE_T<unsigned long> stateVldModHierReq;  /* 0x208 */
    apSTATE_T<unsigned long> stateVldModHier;     /* 0x20C */
    m3dMATR      *pMatrCollidables; /* 0x210 */
    void         *pCollidableData;  /* 0x214 */
#ifdef __cplusplus
    /* Member declarations (canonical set, mirrored from the ws/anim variant of this header).
     * Declared only for C++ callers; the layout above is unchanged so C consumers are unaffected. */
    int      SetObjInstRef();                       // 0x825E2E90
    int      IsValid(int state);                    // 0x825E1130
    char    *GetName();                             // 0x823CD8E0
    void     SetIGNORE_CDT_ALWAYS();                // 0x825E1888
    static void Clear(animINST *pInst);             // 0x825E4028
    m3dBOX  *GetBBox(int forceNoValidate);          // 0x825E4BC8
    cdtSKEL *GetCDTSkeleton();                      // 0x825E1958
    m3dOBB  *GetOBB(int forceNoValidate);           // 0x825E4E30
    void     _GetMatrL2W(m3dMATR *pMatL2W);         // 0x825E5408

    psSECTION GetPS() const;                                // 0x825E18F8 (sret)
    void      LODTerm();                                    // 0x82B0D660
    void      GetPos(m3dV *pos);                            // 0x825E14D8
    void      GetDir(m3dV *dir);                            // 0x825E1538
    void      GetDirUp(m3dV *dir);                          // 0x825E1598
    void      Transform(const m3dMATR *pMTrans, int oper);  // 0x825E28C8
    dsSMART_PTR<rendLIGHT_SET, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> >
              GetLightSet() const;                          // 0x825E3A58 (sret)
    void      SetAffixes(const dsAFFIX_STRING &str);        // 0x825E38A0
    void      LODApply(int visible, float lodDist);         // boundary
    void      UpdateSpeed(float elapsedTime);               // boundary
    int       SetName(const char *name);                    // 0x825E0F58
    int       SetNameClass(const char *name);               // 0x825E0FD0
    int       SetNameTpl(const char *name);                 // 0x825E1048
    objOBJ   *GetObj(int id) const;                         // 0x823CD930
    objOBJ   *GetTopObj() const;                            // 0x823D05C0
    void      DeleteObj(objOBJ *obj);                       // 0x825E2DF8

    void DetachObj(objOBJ *pObj);                   // 0x825E1238
    void Invalidate(int vState, int invalidateCullInfo); // 0x825E20E8
    void UpdateSkinCompOrigin(objOBJ *pSkinCompObj);// 0x82B0F4F0
    void FillObjRendList(bool runtimeUpdate);       // 0x825E2280
    void ValidateHidden();                          // 0x82B107F0
    void LODInit();                                 // 0x82B0EC98
    void Validate(int vState);                      // 0x825E4848

    void Translate(float x, float y, float z, int operOrder);              // 0x825E26D0 boundary
    void Translate(const m3dV *v, int operOrder, int isSpecOpt);           // 0x825E2770 boundary
    void RotateX(float angle, int operOrder);                              // 0x825E2580 boundary
    void RotateY(float angle, int operOrder);                              // 0x825E25F0 boundary
    void RotateZ(float angle, int operOrder);                              // 0x825E2660 boundary
    void Scale(float scale, int operOrder);                                 // 0x823D3400 boundary
    void Scale(float scaleX, float scaleY, float scaleZ, int operOrder);   // 0x825E27E8 boundary

    void     GetMatrL2W(m3dMATR *pMatL2W);          // 0x825E4E80
    // Advance this instance's animation manager by one step; writes pose-change status into `status`.
    // Called from hcex_update_inst_anim. boundary (body external to this batch).
    void     UpdateAnimation(const struct animMNG_UPDATE_INFO &updateInfo, struct animUPDATE_STATUS &status);
    void     ClearIGNORE_CDT_ALWAYS();              // clear the always-ignore-collision layer flag; boundary
    void     FillLinearList();                      // 0x825E1358
    void     FreeCollidables();                     // 0x8291A5F8
    void     PostApplyCreateData(animCREATE_DATA *pCD); // 0x825E1148

    void     ApplyCreateData(animCREATE_DATA *pCD);      // 0x825E4170 boundary
    void     AddLightSet(dsSMART_PTR<rendLIGHT_SET, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > pLightSet); // 0x825E4068

protected:
    void     ApplyModifiers(int stateVld);          // 0x825E4720 (protected, IAA)
    animINST();                                     // 0x825E3E60
    ~animINST();                                    // 0x825E3908

private:
    void     ApplyAnim();                           // 0x82B2BD18 (private)
    void     CalcVisDistFactor();                   // 0x825E1828 (private)
    void     CalcOrthoL2W(m3dMATR *matrOrthoL2W);   // 0x825E2BE0 (private)
    int      UpdateValidateState(int state);        // 0x825E1FB0 (private)
    int      UpdateInvalidateState(int vState);     // 0x825E1E38 (private, AAA)
    void     ValidateOBB();                         // 0x825E3220 (private, AAA)
#endif
} animINST; /* 536 bytes */

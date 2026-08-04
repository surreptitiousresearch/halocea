#pragma once
#include "../ap/apSTATE_T.h"
#include "../m3d/m3dMATR.h"
#include "../ps/psSECTION.h"
#include "../ds/dsAFFIX_STRING.h"
#include "objGEOM_UNSHARED.h"
#include "../../animINST.h"
// ws-engine obj: geometry/model-object class (vertices, faces, bones, modifiers,
// hierarchy, transforms). DB-verified layout (types_members objOBJ) — size 220 (0xDC).

struct objFACE;      // objFACE.h — pointer only (GetFace return)
struct objMODIFIER;  // objMODIFIER.h — pointer only
struct m3dV;         // ../m3d/m3dV.h — pointer only (vertex/normal list element)

#ifndef objOBJ_DEFINED
#define objOBJ_DEFINED
typedef struct objOBJ {
    // Condition callback registered by SetStateProcYes/No.
    typedef int (*StateCondProc)(objOBJ *obj, void *usr);

    // Pluggable per-object-data allocator/deleter hooks (static). When non-null they override
    // the default StdAllocObjData path. DB: ?dataAlloc@/?dataFree@objOBJ (0x842264C0/0x842264C4).
    typedef void *(*DataAllocProc)(objOBJ *obj, int data, int size, void *oldData);
    typedef void  (*DataFreeProc)(objOBJ *obj, void *oldData, int data);
    static DataAllocProc dataAlloc; // 0x842264C0
    static DataFreeProc  dataFree;  // 0x842264C4

    char              *name;              // 0x00
    __int16            id;                // 0x04
    unsigned char _pad0[2]; /* db-verified padding */
    objGEOM_UNSHARED  *pGeom;             // 0x08
    int                nFace;             // 0x0C
    int                nVert;             // 0x10
    animINST          *pInst;             // 0x14
    int                state;             // 0x18
    int                stateProc;         // 0x1C
    apSTATE_T<__int64> stateUsr64;        // 0x20
    apSTATE_T<__int64> stateShare;        // 0x28
    apSTATE_T<unsigned char> stateVis;    // 0x30
    unsigned char _pad1[3]; /* db-verified padding */
    dsAFFIX_STRING     affixes;           // 0x34
    unsigned int       lastFameVisible;   // 0x38
    objOBJ            *parent;            // 0x3C
    m3dMATR            matrLT;            // 0x40
    objOBJ            *next;              // 0x80
    objOBJ            *prev;              // 0x84
    objOBJ            *child;            // 0x88
    m3dMATR            matrModel;         // 0x8C
    m3dMATR           *pMatrLTPrev;       // 0xCC
    psSECTION          ps;                // 0xD0
    __int16            animNmb;           // 0xD4
    unsigned __int16   stateRend;         // 0xD6
    int                dbgRenderRefCount; // 0xD8

    // ---- reversed in the ws_obj_0001 batch ----
    void SetStateProcYes(int stateProc, StateCondProc cbCond);      // 0x826845E0
    void SetStateProcNo(int stateProc, StateCondProc cbCond);       // 0x826845F0
    static void Delete(objOBJ *obj);                                // 0x82A7EB40
    static objOBJ *Make();                                          // 0x82A7E300
    void _FreeModifierList();                                       // 0x82A7E560
    void _FreeName();                                               // 0x82A7EB80
    void _FreeGeom();                                               // 0x82A7EDD8
    int  _AllocName(int nameLen);                                   // 0x82A7E3A8
    void SetInterleavedData(void *interleavedData, int size,
                            unsigned __int64 fvf);                  // 0x82BCB5D0
    void *GetVertBoneIdxList(int offset);                           // 0x82BCC9F0
    void *GetVertBoneWeightList(int offset);                        // 0x82BCC998
    void SetVertBoneIdxList(unsigned __int8 *idxList);             // 0x82BCB3E8
    int  GetFaceSize() const;                                       // 0x82A7F740
    void SetRenderPassId(int passId);                              // 0x826823C0
    void GetOBB(struct m3dOBB *obb);                                // boundary (used by rendCullAndValidateInst)

    // ---- reversed in the ws_obj_0002 batch ----
    void FreeUnnecessaryData();                                     // 0x82A7F5C0
    int  HaveAffix(const char *aff, int asSubString) const;         // 0x82684D60
    static void *StdAllocObjData(objOBJ *pObj, int data,
                                 unsigned int size, void *oldData); // 0x82A7E5A8
    void SetGeomSharingOfsVtx(int ofs);                            // 0x826603B8
    void SetGeomSharingOfsFace(int ofs);                           // 0x82660440
    int  GetGeomSharingsOfsVtx() const;                             // 0x82660298
    int  GetGeomSharingsOfsFace() const;                            // 0x82660328
    m3dVTX *GetTexCoordList(unsigned int tcInd);                    // 0x826601C0
    void SetColorMPHier(unsigned int color);                        // 0x826838C0
    void SetTransparency(unsigned char transp);                     // 0x82682110
    void _UnshareGeom();                                            // 0x82A7F798
    void SetVertBoneWeightList(float *weightList);                  // 0x82BCC900 (mangle PAX=void*)
    int  GetNObj(StateCondProc cbCond);                             // 0x82681AF0

    // ---- reversed in the ws_obj_0003 batch ----
    void  RemoveModifiers();                                        // 0x82A8AD28
    int   ApplyModifiers(int stateVldMask);                         // 0x82A8AE30
    void  SetStateProc(int stateProc, int enable, StateCondProc cbCond); // 0x82682430
    void *AllocObjData(unsigned int data, int size, void *oldData); // 0x82A7E640
    void  FreeObjData(unsigned int data, void *oldData);            // 0x82A7E6E0
    int   _AllocVertTangentList(int tcInd, int nVert);             // 0x82A7F508
    void  SetGeomSharingHost(objOBJ *pObj);                         // 0x82BCB288
    void  SetStateRendNo(int stateBits);                            // boundary — set render-state flags (0x100 = material override applied)
    void  SetVertColorList(unsigned int vcInd, unsigned int *list); // 0x82BCB450
    objFACE *GetFace(int faceNmb);                                 // 0x82661278
    int   _UnshareFaceSplit();                                      // 0x82A80728
    int   _UnshareName();                                           // 0x82A7EBF0
    void  _UnshareInterleavedData();                               // 0x82A80288

    // ---- reversed in the ws_obj_0004 batch ----
    int   _AllocColorList(unsigned int vcInd, int nVert);          // 0x82A7F348
    int   _AllocVertNormalList(int nVert);                         // 0x82A7F420
    int   _AllocVertList(int nVert);                               // 0x82A7EFD0
    int   _AllocGeom();                                            // 0x82A7ECC0
    void  _AllocInterleavedData(int nVert, unsigned char size,
                                unsigned __int64 fvf);             // 0x82A7F638
    int   _UnshareFaceList();                                      // 0x82A805D0
    int   _UnshareColorList(unsigned int vcInd);                   // 0x82A7FBB8
    int   _UnshareVertNormalList();                               // 0x82A7FD38
    int   _UnshareVertTangentList(int tcInd);                     // 0x82A7FEE8
    int   _UnshareVertList();                                      // 0x82A7F830
    void  _UnshareInstancedData();                                // 0x82A80198
    void  SetVertNormalList(m3dV *list);                          // 0x82BCCB40
    void  SetVertList(m3dV *list);                                // 0x82BCC7E0
    objOBJ *Duplicate(objOBJ *pPar);                             // 0x826621B0

    // ---- reversed in the ws_obj_0005 batch ----
    int  _AllocTexCoord(unsigned int tcInd, int nVert);            // 0x82A81B08
    int  _UnshareTexCoord(unsigned int tcInd);                     // 0x82A81C40
    void _FreeTexCoord(unsigned int tcInd);                        // 0x82A81008
    void SetTexCoordList(unsigned int tcInd, m3dVTX *ptr);         // 0x82BCCD18
    void SetVertTangentList(unsigned int tcInd, void *tangentList);// 0x82BCCFA8
    int  _AllocFaceList(int nFace);                                // 0x82A803A0
    void _AllocInstancedData(unsigned __int16 num, unsigned char size,
                             unsigned __int64 fvf);                // 0x82A80008
    void Detach();                                                 // 0x826605C8
    int  _UnshareAll();                                            // 0x82660760
    void _UnshareVertBoneList();                                   // 0x82A7F960
    void _AllocVertBoneList(int nVertNew);                         // 0x82A7F0F0
    objOBJ *_DuplicateTpl2Inst(int isShareGeom);                   // 0x82661600

    // ---- reversed in the ws_obj_0006 batch ----
    void _DestroyData();                                           // 0x82661B08

    // ---- same-class methods called but external to this batch ----
    objOBJ();                                                       // boundary (ctor)
    ~objOBJ();                                                      // boundary (dtor)
    objOBJ &operator=(const objOBJ &src);                          // boundary (copy-assign)
} objOBJ;
#endif

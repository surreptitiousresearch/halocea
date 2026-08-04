#pragma once
/* Boundary declarations for the HCEX "halo library" SSL callbacks (cbHcexHideLibrary /
 * cbHcexShowLibrary). The library is a single scripted entENTITY, lazily created on first Show,
 * and driven through SSL functions whose resolved ids live in the dsSTRID globals below. The ws
 * entity/scene/camera/math systems are canonical headers; only the HCEX-owned globals and the
 * few free functions the bridge calls are declared here. Source: halo_cine.cpp. */

#include "../ws/ent/entENTITY.h"          /* entENTITY (Hide/Show, sslObject, pInst) */
#include "../ws/ds/WEAK_PTR.h"            /* ds::WEAK_PTR<entENTITY> */
#include "../ws/ds/dsSTRID.h"             /* dsSTRID */
#include "../ws/ssl/sslOBJ_REF.h"         /* sslOBJ_REF (CallFunc) */
#include "../entCREATE_DATA.h"            /* entCREATE_DATA (name via animCREATE_DATA base) */
#include "../ws/obj/obj_free_functions.h"/* objFindName */
#include "../ws/obj/objOBJ.h"            /* objOBJ (matrLT) */
#include "../ws/cam/camCAMERA.h"          /* camCAMERA::SetParameters */
#include "../ws/m3d/m3d_boundary.h"       /* _m3dCheckValid, _m3dNegateVector, m3dNormalize */
#include "../ws/anim/animINST.h"          /* animINST (pObj) */

/* --- scene / farm systems (opaque; touched only through the helpers below) --- */
typedef struct scnSCENE  scnSCENE;   /* ws scene root — boundary */
typedef struct farmSYSTEM farmSYSTEM; /* ws farm system — boundary (isSuspendAdd latch only) */
typedef struct msgDATA   msgDATA;    /* ws message payload — boundary */

/* --- HCEX-owned globals --- */
extern ds::WEAK_PTR<entENTITY> hcexLibrary;      /* the lazily-created scripted library entity */
extern dsSTRID                 hcexLibraryInit;   /* SSL func id: library init */
extern dsSTRID                 hcexLibraryShow;   /* SSL func id: library show */
extern dsSTRID                 hcexLibraryHide;   /* SSL func id: library hide */
extern unsigned char           hcex_off_cam;      /* latch: script camera override active */
extern scnSCENE               *gsScenePtr;        /* active ws scene */
extern farmSYSTEM             *farmSys;           /* ws farm system singleton */

/* --- ws / bridge free functions (boundary) --- */
extern entENTITY *entCreate(scnSCENE *scene, const char *name, const char *parent,
                            entCREATE_DATA *data, msgDATA *msg);
extern unsigned char *farmSYSTEM_isSuspendAdd(farmSYSTEM *sys); /* &sys->isSuspendAdd */
extern int        gsCameraCount(void);
extern camCAMERA *gsCameraGet(int idx);

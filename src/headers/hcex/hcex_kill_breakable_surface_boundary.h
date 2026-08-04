#pragma once
/* Boundary declarations for hcex_kill_breakable_surface.c.
 *
 * This is HCEX bridge glue that reaches into the ws-engine scene/SSL object model to destroy the scene
 * instance that visually represents a breakable surface. The ws containers (dsVECTOR), the scene instance
 * create-data, the SSL object/reference system and the anim/ent object model are all extern boundaries per
 * the project's re-source conventions — declared here as boundaries. Only fields actually touched are
 * modeled, at their database offsets. */

typedef struct entENTITY entENTITY;   /* opaque; sslObject lives at +0x58 */
typedef struct sslOBJECT sslOBJECT;

#include "hcex_ds_boundary.h"   /* dsTSTRING<char> (ref-counted buffer view) */
#include "../ws/m3d/m3dMATR.h"
#include "../ws/ds/dsAFFIX_STRING.h"
#include "../ws/ps/psSECTION.h"

/* sslERROR (12 bytes; DB-verified, types_members sslERROR): id@0 (sslERROR_ID), desc@4
 * (dsTSTRING<char>), tags@8. CallFunc returns one by value (sret). */
typedef int sslERROR_ID; /* ws/ssl/sslERROR.h has the DB enumerator set */
typedef struct sslERROR
{
    sslERROR_ID id;    /* 0x00 */
    dsTSTRING<char>   desc;  /* 0x04 */
    int         tags;  /* 0x08 */
} sslERROR;

/* animINST — canonical full DB-verified layout (pEnt@0x0C); the former per-consumer partial
 * slice was consolidated. */
#include "../animINST.h"

typedef int dsCMP;

/* scnINST_CREATE_DATA — 88-byte scene-instance creation descriptor (DB-verified,
 * types_members scnINST_CREATE_DATA). */
typedef struct scnINST_CREATE_DATA
{
    int             state;     /* 0x00 */
    dsTSTRING<char>       nameInst;  /* 0x04 */
    dsTSTRING<char>       nameTpl;   /* 0x08 */
    dsTSTRING<char>       nameClass; /* 0x0C */
    m3dMATR         matrInst;  /* 0x10 (64B) */
    dsAFFIX_STRING  affixes;   /* 0x50 */
    psSECTION       ps;        /* 0x54 */
} scnINST_CREATE_DATA;

/* hcexBREAKABLE_SURF — key + scene instance for one breakable surface (sorted vector element). */
typedef struct hcexBREAKABLE_SURF
{
    int                  bsp;    /* 0x00 */
    int                  idx;    /* 0x04 */
    animINST            *pInst;  /* 0x08 */
    scnINST_CREATE_DATA  cd;     /* 0x0C */
} hcexBREAKABLE_SURF;

/* dsSTRID — interned string id — comes from hcex_ds_boundary.h. */

/* sslOBJ_REF — handle to an SSL object (DB-verified, types_members sslOBJ_REF: pObject@0 —
 * size 4). CallFunc returns an sslERROR by value (see above), not an sslOBJ_REF. */
typedef struct sslOBJ_REF { sslOBJECT *pObject; } sslOBJ_REF;

/* dsVECTOR<hcexBREAKABLE_SURF,8> — sorted vector of surfaces (opaque). */
typedef struct dsVECTOR_BSURF dsVECTOR_BSURF;
extern dsVECTOR_BSURF hcexBreakableSurfaces;

/* one-time-initialized "Destroy" SSL function name + its init guard. */
extern dsSTRID fnDestroy;
extern unsigned int hcex_destroy_strid_initialized;   /* the _S1 static guard the decompiler shows */

/* boundary ops (free-function form of the C++ thiscalls). */
extern void  scnINST_CREATE_DATA_ctor(scnINST_CREATE_DATA *cd);
extern void  scnINST_CREATE_DATA_dtor(scnINST_CREATE_DATA *cd);
extern int   dsVECTOR_BSURF_FindSorted(dsVECTOR_BSURF *v, const hcexBREAKABLE_SURF *key, dsCMP *cmp);
extern hcexBREAKABLE_SURF *dsVECTOR_BSURF_index(dsVECTOR_BSURF *v, int i);
extern void  dsSTRID_init(dsSTRID *s, const char *name, int flags);
extern void  sslOBJ_REF_ctor(sslOBJ_REF *r, sslOBJECT *obj);
extern void  sslOBJ_REF_CallFunc(sslERROR *result, sslERROR *target, dsSTRID *func,
        int argc, void *args, void *err, sslOBJ_REF *self);
extern sslOBJECT *entENTITY_get_sslObject(entENTITY *e);   /* &e->sslObject (entENTITY + 0x58) */
extern void dlFree(void *ptr);

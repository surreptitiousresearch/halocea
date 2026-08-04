#pragma once
/* Boundary types/functions for hcex_create_effect_impl. The ws-engine entity/scene, sound, and string
 * types — plus the apCOUNTER performance-profiler instrumentation that brackets the body — are declared
 * here as boundaries (bodies outside this re-source). */

#include "hcex_prepare_locations_boundary.h"   /* m3dV, m3dMATR, animINST */
#include "hcex_create_effect_boundary.h"       /* dsTSTRING_flat, dsTSTRING_BUF_HEADER, entENTITY,
                                                           snd_BUFFER, dsSTRID */

/* animCREATE_DATA — the anim-instance creation descriptor entCREATE_DATA derives from
 * (232 bytes, DB-verified types_members animCREATE_DATA). Flat plain-C model consistent with
 * this boundary file's convention. */
struct animCREATE_DATA_vtbl; struct rendSR_DATA;
#include "../ws/ps/psSECTION.h"      /* canonical psSECTION (4B) — avoids C2011 */
#include "../ws/ds/dsAFFIX_STRING.h" /* canonical dsAFFIX_STRING (dsTSTRING<char> @0 — 4B);
                                        * replaces the former flat {dsTSTRING_flat base} model */
typedef struct animCREATE_DATA
{
    animCREATE_DATA_vtbl *__vftable;         /* 0x00 */
    m3dMATR               matrInst;          /* 0x04 */
    float                 frameCur;          /* 0x44 */
    int                   state;             /* 0x48 */
    psSECTION             ps;                /* 0x4C */
    dsAFFIX_STRING        affixes;           /* 0x50 */
    char                  name[128];         /* 0x54 */
    __int16               animSeqNmb;        /* 0xD4 */
    unsigned __int8       animSeqIsCycled;   /* 0xD6 */
    unsigned __int8       isIgnorePS_Scale;  /* 0xD7 */
    rendSR_DATA          *srData;            /* 0xD8 */
    float                 scaleX;            /* 0xDC */
    float                 scaleY;            /* 0xE0 */
    float                 scaleZ;            /* 0xE4 */
} animCREATE_DATA;                           /* 232 bytes */

/* entCREATE_DATA — entity creation descriptor: animCREATE_DATA base@0 + pDomSpawn@0xE8
 * (DB-verified types_members entCREATE_DATA) — size 236. */
typedef struct entCREATE_DATA
{
    animCREATE_DATA base;      /* 0x00 (anonymous DB base) */
    struct gsDOM_SPAWN *pDomSpawn; /* 0xE8 */
} entCREATE_DATA;

extern void entCREATE_DATA_ctor(entCREATE_DATA *d);
extern void entCREATE_DATA_dtor(entCREATE_DATA *d);

typedef struct scnSCENE scnSCENE;
typedef struct msgDATA  msgDATA;
extern scnSCENE *gsScenePtr;
extern entENTITY *entCreate(scnSCENE *scene, const char *name, const char *parent,
        entCREATE_DATA *data, msgDATA *msg);

/* SSL / farm systems whose "suspend add" flag is briefly forced during entCreate. These are
 * large ws-engine systems (gsSSL_SYSTEM=200B / farmSYSTEM=292B) touched here only through their
 * `isSuspendAdd` flag; kept opaque with byte-flag accessors per this corpus's boundary
 * convention rather than modeling the full systems. */
typedef struct gsSSL_SYSTEM gsSSL_SYSTEM;
typedef struct farmSYSTEM   farmSYSTEM;
extern gsSSL_SYSTEM *gsSslSystem;
extern farmSYSTEM   *farmSys;
extern unsigned char *gsSSL_SYSTEM_isSuspendAdd(gsSSL_SYSTEM *sys); /* &sys->isSuspendAdd */
extern unsigned char *farmSYSTEM_isSuspendAdd(farmSYSTEM *sys);     /* &sys->isSuspendAdd */

/* sound system used for the plasma-grenade detonation cue. snd_BUFFER_PARAMS /
 * snd_BUFFER_PARAMS_FULL / snd_BUFFER / snd_SYSTEM come from hcex_create_effect_boundary.h
 * (included above) — the single shared DB-verified definitions. */
extern void snd_BUFFER_PARAMS_ctor(snd_BUFFER_PARAMS_FULL *p);

extern snd_SYSTEM *snd_System;
extern snd_BUFFER *snd_System_GetBuffer(snd_SYSTEM *sys, dsSTRID *id, snd_BUFFER_PARAMS_FULL *params);
extern void snd_BUFFER_Play(snd_BUFFER *b);
extern void snd_BUFFER_Release(snd_BUFFER *b);

extern void dsSTRID_ctor(dsSTRID *s, const char *name, int flags);

extern const m3dV m3dVUnitY;
extern void m3dMATR_MakeLCS2WCS_VY(m3dMATR *out, const m3dV *origin, const m3dV *up);

extern void dlFree(void *ptr);
extern int sprintf_0(char *string, const char *format, ...);

extern int effectId;

/* apCOUNTER profiler scope (ws-engine instrumentation) bracketing the function body. The original inlines
 * the apCOUNTER macro for counter units 0 and 1; reproduced here as enter/exit boundary calls. */
extern void cnt_hcex_create_sfx_enter(void);
extern void cnt_hcex_create_sfx_exit(void);

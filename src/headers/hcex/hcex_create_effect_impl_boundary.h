#pragma once
/* Boundary types/functions for hcex_create_effect_impl. The ws-engine entity/scene, sound, and string
 * types — plus the apCOUNTER performance-profiler instrumentation that brackets the body — are declared
 * here as boundaries (bodies outside this re-source). */

#include <stdint.h>
#include "hcex_prepare_locations_boundary.h"   /* m3dV, m3dMATR, animINST */
#include "hcex_create_effect_boundary.h"       /* dsTSTRING_flat, dsTSTRING_BUF_HEADER, entENTITY,
                                                           snd_BUFFER, dsSTRID */

/* animCREATE_DATA (232B) / entCREATE_DATA (236B) — the creation descriptors.
 *
 * DEVIATION (2026-08-07, odr_dup drain): this header used to restate BOTH bodies as flat plain-C
 * structs beside their canonical homes (ws/anim/animCREATE_DATA.h, entCREATE_DATA.h) — two
 * `error: redefinition`s in header_layout's probe TU. The canonical bodies win on DB evidence:
 *   - animCREATE_DATA: types_members lists 14 members, identical names/offsets/types to the copy
 *     here (__vftable@0 … scaleZ@228), so the copy carried nothing the canonical lacks; the
 *     canonical additionally carries the DB-verified ctor/dtor/Clear/ApplySRData addresses.
 *   - entCREATE_DATA: types_members row 0 is an ANONYMOUS BASE-CLASS member of type
 *     animCREATE_DATA at offset 0 (is_baseclass=1, size 232), which `struct entCREATE_DATA :
 *     animCREATE_DATA` models and the flat `animCREATE_DATA base;` member here did not. That is
 *     the one spelling difference, and it is why hcex_create_effect_impl.cpp lost its `.base.`
 *     hops in the same change.
 * psSECTION / dsAFFIX_STRING / m3dMATR / rendSR_DATA now come in through the canonical header. */
#include "../ws/anim/animCREATE_DATA.h"
#include "../entCREATE_DATA.h"

/* entCREATE_DATA::entCREATE_DATA / ~entCREATE_DATA — 0x8252FFB8 / 0x823CE690. The .cpp now
 * declares the descriptor by value and lets the real ctor/dtor run (which is what the binary
 * emits: `bl entCREATE_DATA::entCREATE_DATA(void)` @0x823DE3EC and
 * `bl animCREATE_DATA::~animCREATE_DATA(void)` @0x823DE73C, the base dtor the derived one folds
 * into), so the flattened free-function shims that used to be declared here are gone. */

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

extern "C" void dlFree(void *ptr);
extern "C" int sprintf_0(char *string, const char *format, ...);

extern "C" int effectId;

/* apCOUNTER profiler scope (ws-engine instrumentation) bracketing the function body. The original inlines
 * the apCOUNTER macro for counter units 0 and 1; reproduced here as enter/exit boundary calls. */
extern void cnt_hcex_create_sfx_enter(void);
extern void cnt_hcex_create_sfx_exit(void);

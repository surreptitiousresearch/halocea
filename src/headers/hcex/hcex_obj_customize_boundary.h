#pragma once
/* Boundary declarations for hcex_obj_customize.c.
 *
 * hcex_obj_customize retargets a tracked HCEX object's skin/preset to match a tag name. It walks the
 * ws-engine property model: the entity embeds a prop "description" subobject that is polymorphic — either a
 * propINST_CONSTRUCTOR_DESC (a single constructor) or a propCONTAINER_DESC (a container whose child property
 * arrays are searched for the first propINST_CONSTRUCTOR). All prop* / dsTYPE_ID / dsVECTOR types are ws-engine
 * C++ classes treated as extern boundaries; only the fields this bridge touches are modeled, at their
 * database offsets (propINST_CONSTRUCTOR is 48 bytes: typeId@0x18, curPresetName@0x28, mSeed@0x2C). */

#include "../apCL.h"
#include "HCEX_OBJ.h"          /* HCEX_OBJ (id, pInst) */
#include "hcex_ds_boundary.h"  /* dsTSTRING<char>, dsTSTRING_UnsafeInit, dlFree */

typedef struct entENTITY entENTITY;   /* opaque ws-engine entity; embeds the prop DESC at +0x60 */

/* animINST — canonical full DB-verified layout (pEnt@0x0C); the former per-consumer partial
 * slice was consolidated. */
#include "../animINST.h"
#include "../ws/ds/dsFIXED_BIT_ARRAY.h"
#include "../ws/ds/WEAK_PTR.h"
#include "../ws/ds/dsSMART_PTR.h"
#include "../ws/ssl/sslOBJ_REF.h"

struct propBASE_DESC; /* boundary — property descriptor */

/* runtime type identity (16 bytes; DB: dsFIXED_BIT_ARRAY<4> id@0) */
#include "../ws/ds/dsTYPE_ID.h" // canonical — avoids C2011 when co-included with ws/ds/dsTYPE_ID.h

/* propBASE / propENT — canonical ws headers (layout-identical: propBASE 40B, propENT 40B).
 * Included before the dsVECTOR_propBASE shim (which needs propBASE declared) and to avoid C2011
 * when the real prop headers arrive via animINST.h. */
#include "../ws/prop/propENT.h"

/* dsVECTOR_propBASE is the plain-C spelling of the DB instantiation dsVECTOR<propBASE *,8>
 * (20 bytes; no C-spellable DB name of its own); DSVECTOR_INSTANCE keeps the layout verifier
 * from mis-binding this instantiation spelling. */
#define DSVECTOR_INSTANCE struct
typedef DSVECTOR_INSTANCE dsVECTOR_propBASE
{
    propBASE **pData;     /* 0x00 */
    int        nElem;     /* 0x04 */
    int        allocated; /* 0x08 */
    apCL       __cl;      /* 0x0C */
} dsVECTOR_propBASE;


/* propINST_CONSTRUCTOR (48 bytes, database layout: anonymous propENT base@0 (40B),
 * curPresetName@0x28, mSeed@0x2C) — the canonical ws definition is used directly; types_members
 * marks the propENT slot is_baseclass=1, so the inheritance form is the faithful one and typeId
 * is reached as `p->typeId` (it lives at +0x18 of the propBASE layer). In the container-DESC
 * variant the curPresetName/mSeed region (0x28..) is reinterpreted as the two dsVECTOR<propBASE*>
 * child arrays hcex_obj_customize iterates. */
#include "../ws/prop/propINST_CONSTRUCTOR.h"

/* --- runtime type-id singletons (const dsTYPE_ID) --- */
extern const dsTYPE_ID propINST_CONSTRUCTOR_DESC_TYPE_ID;   /* propINST_CONSTRUCTOR_DESC::TYPE_ID */
extern const dsTYPE_ID propCONTAINER_DESC_TYPE_ID;          /* propCONTAINER_DESC::TYPE_ID */

/* --- ws property-model methods (free-function form of the C++ thiscall) --- */
extern int  dsTYPE_ID_IsDerivedFrom(const dsTYPE_ID *self, const dsTYPE_ID *base);
extern propBASE **dsVECTOR_propBASE_index(dsVECTOR_propBASE *v, int i);              /* operator[] */
extern propINST_CONSTRUCTOR *propBASE_GetProperty_INST_CONSTRUCTOR(propBASE *self);  /* GetProperty<> */
extern int  propINST_CONSTRUCTOR_GetPresetCount(propINST_CONSTRUCTOR *self);
extern const dsTSTRING<char> *propINST_CONSTRUCTOR_GetPresetName(propINST_CONSTRUCTOR *self, int index);
extern void propINST_CONSTRUCTOR_ChangeSkin(propINST_CONSTRUCTOR *self, const dsTSTRING<char> *preset);
extern int  dsTSTRING_IsStartWith(const dsTSTRING<char> *self, const char *s, int noCase, int ofs);

/* --- hcex bridge helpers / CRT --- */
extern HCEX_OBJ    *hcex_obj_get(int id);
extern "C" const char  *hcex_tag_get_name(int tag_index);
extern "C" char        *hcex_conv_name(char *name, int max_len);
extern "C" char        *strrchr(const char *s, int ch);
extern "C" char        *strncpy(char *dst, const char *src, unsigned int n);

#pragma once
/* Boundary declarations for hcex_kill_breakable_surface.c.
 *
 * This is HCEX bridge glue that reaches into the ws-engine scene/SSL object model to destroy the scene
 * instance that visually represents a breakable surface.
 *
 * DEVIATION (2026-08-07, odr_dup drain): this header used to restate FOUR types that already had
 * canonical, DB-verified homes elsewhere in the corpus — sslERROR, scnINST_CREATE_DATA,
 * hcexBREAKABLE_SURF and sslOBJ_REF — i.e. four `error: redefinition`s in header_layout's probe TU,
 * and four types whose layout a TU resolved by include order. All four copies agreed with the DB on
 * member offsets, so nothing was lost by deleting them, with ONE exception that was a real defect:
 * this header spelled `typedef int sslERROR_ID;` where types_members sslERROR reports member `id` as
 * an ENUM (member_type sslERROR_ID, mt_is_enum=1) whose full enumerator set the canonical
 * ws/ssl/sslERROR.h carries. The canonical headers are included instead:
 *   - ws/ssl/sslERROR.h        sslERROR: id@0 (sslERROR_ID), desc@4 (dsTSTRING<char>), tags@8 — 12B
 *   - ws/ssl/sslOBJ_REF.h      sslOBJ_REF: pObject@0 — 4B
 *   - ws/scn/scnINST_CREATE_DATA.h  (via hcex_surf_vector.h -> hcex_breakable_surf.h) — 88B
 *   - hcex/hcex_breakable_surf.h    hcexBREAKABLE_SURF: bsp@0, idx@4, pInst@8, cd@12 — 100B
 *
 * The flattened `(out, self, ...)` sret shims went with them: hcex_kill_breakable_surface @0x823B20C8
 * calls the ordinary C++ forms — `bl scnINST_CREATE_DATA::scnINST_CREATE_DATA(void)`,
 * `bl dsVECTOR<hcexBREAKABLE_SURF,8>::FindSorted<dsCMP,hcexBREAKABLE_SURF>(...)`,
 * `bl dsVECTOR<hcexBREAKABLE_SURF,8>::operator[](int)`, `bl sslOBJ_REF::sslOBJ_REF(sslOBJECT *)`,
 * `bl sslOBJ_REF::CallFunc(dsSTRID,int,dsDATA *,dsDATA *,sslOBJ_REF)`, `bl dlFree`,
 * `bl scnINST_CREATE_DATA::~scnINST_CREATE_DATA(void)` — with no separate `~sslOBJ_REF` because MSVC
 * has the CALLEE destroy a by-value class parameter.
 *
 * The container is now `hcex_surf_vector` (= dsVECTOR<hcexBREAKABLE_SURF,8>) rather than a local
 * opaque `dsVECTOR_BSURF`, which also makes this TU's `hcexBreakableSurfaces` extern agree with its
 * two siblings (hcex_init_breakable_surfaces.cpp, hcex_is_breakable_surface.cpp) — they already
 * declared it at the canonical type while this one declared a different one. */

#include "hcex_ds_boundary.h"          /* dsTSTRING<char>, dsSTRID (ref-counted buffer view) */
#include "hcex_surf_vector.h"          /* hcex_surf_vector = dsVECTOR<hcexBREAKABLE_SURF,8>;
                                        * pulls hcexBREAKABLE_SURF, scnINST_CREATE_DATA, animINST */
#include "../ws/ssl/sslERROR.h"        /* sslERROR / sslERROR_ID — the real DB enumerator set */
#include "../ws/ssl/sslOBJ_REF.h"      /* sslOBJ_REF + CallFunc(dsSTRID, ...) @0x82526578 */

typedef struct entENTITY entENTITY;    /* opaque; sslObject lives at +0x58 */

/* stateless comparator selector for FindSorted (an empty predicate; DB dsCMP) — same local
 * spelling the sibling hcex_is_breakable_surface.cpp uses. */
#include "../ws/ds/dsCMP.h"   /* canonical dsCMP (stateless empty struct) — a local `typedef int dsCMP` is a C2371/ODR conflict with it */

extern hcex_surf_vector hcexBreakableSurfaces;

/* one-time-initialized "Destroy" SSL function name + its init guard. */
extern "C" dsSTRID fnDestroy;
extern unsigned int hcex_destroy_strid_initialized;   /* the _S1 static guard the decompiler shows */

/* boundary ops (free-function form of the C++ thiscalls). */
extern void dsSTRID_init(dsSTRID *s, const char *name, int flags);
/* &e->sslObject (entENTITY + 0x58). DEVIATION: previously declared as returning `sslOBJECT *` and
 * then cast to `sslERROR *` at the call site — both wrong. The slot is an sslOBJ_REF, which is what
 * sslOBJ_REF::CallFunc is invoked on in the disassembly. */
extern sslOBJ_REF *entENTITY_get_sslObject(entENTITY *e);

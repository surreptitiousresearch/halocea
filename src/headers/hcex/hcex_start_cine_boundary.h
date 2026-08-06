#pragma once
/* Boundary declarations for hcex_start_cine.c — the HCEX cinematic-capture setup path. The per-actor hidden
 * containers, the cine-process entry point and the debug-dump record are ws-engine / hcex-bridge state
 * modeled as extern boundaries; only the touched shape is declared. */

#include "DBG_CINE_INFO.h"   /* DBG_CINE_INFO, dsTSTRING_flat (via hcex_ds_boundary) */

/* canonical dsVECTOR<T,8> (only nElem is written directly by this file). The former flat
 * dsVECTOR_hdr view is kept as an element-erased alias. */
#include "../ws/ds/dsVECTOR.h"
/* dsSTRID comes flat via hcex_ds_boundary.h (DBG_CINE_INFO.h include chain) */
typedef dsVECTOR<void *, 8> dsVECTOR_hdr; /* element-erased view */

extern DBG_CINE_INFO _dbgCineInfo;
extern dsVECTOR<dsTSTRING<char>, 8> cineHiddenActorsTplNames; /* DB dsVECTOR<dsTSTRING<char>,8> */
extern dsVECTOR<dsSTRID, 8>         cineHiddenHcexObjKeys;    /* DB dsVECTOR<dsSTRID,8> */

extern "C" const char * hcex_get_parent_script(void);
extern void dsVECTOR_tstring_Clear(dsVECTOR<dsTSTRING<char>, 8> *v); /* dsVECTOR<dsTSTRING<char>,8>::Clear */
extern void hcex_cine_process_start(const dsTSTRING_flat *parentScript);
extern void DBG_CINE_INFO_Init(DBG_CINE_INFO *self);
extern void dsTSTRING_assign(dsTSTRING_flat *dst, const dsTSTRING_flat *src);   /* dsTSTRING_flat::operator= */

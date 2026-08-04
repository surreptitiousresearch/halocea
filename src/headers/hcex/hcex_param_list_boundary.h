#pragma once
/* Boundary declarations for the ws-engine parameter-list containers used by the HCEX event-fire helpers
 * (hcex_fire_event_i, hcex_fire_event_s, hcex_fire_plr_event, hcex_load_level).
 *
 * A parameter is a dsSTRID key + a tagged value (dsDATA); parameters are collected either in a fixed
 * dsSTATIC_PARAM_LIST<N> or a growable dsVECTOR_PARAM_LIST, then handed to dsEVENT_MGR::SignalEvent. All
 * layouts verified via types_members. Per the project's re-source conventions these ws-engine types are
 * extern boundaries: only the touched shape is modeled and the C++ template "methods" are declared as free
 * functions whose first parameter is the object (mirroring the compiler-generated thiscall). */

#include "hcex_ds_boundary.h"   /* dsSTRID, dsTSTRING_flat, dsEVENT_MGR, gEventMgr, dlFree */
#include "../ws/ds/dsVECTOR.h"  /* dsVECTOR<T,N> (canonical template; pulls in apCL) */
/* Include canonical ds param containers rather than redefining them locally (avoids C2011 when a
 * TU also pulls the ws/ds headers directly). Layouts are identical to the former local models. */
#include "../ws/ds/dsDATA.h"              /* dsDATA, dsDATA_TYPE */
#include "../ws/ds/dsPARAM.h"             /* dsPARAM */
#include "../ws/ds/dsPARAM_LIST.h"        /* dsPARAM_LIST */
#include "../ws/ds/dsSTATIC_PARAM_LIST.h" /* dsSTATIC_PARAM_LIST<N> */
#include "../ws/ds/dsVECTOR_PARAM_LIST.h" /* dsVECTOR_PARAM_LIST */

typedef dsSTATIC_PARAM_LIST<1> dsSTATIC_PARAM_LIST_1; /* 20 bytes */
typedef dsSTATIC_PARAM_LIST<2> dsSTATIC_PARAM_LIST_2; /* 32 bytes */

typedef dsVECTOR<dsPARAM, 8> dsVECTOR_dsPARAM;        /* 20 bytes — DB dsVECTOR<dsPARAM,8> */

/* --- methods (free-function form of the compiler-generated thiscall) --- */
extern void dsSTRID_ctor_default(dsSTRID *s);                                /* dsSTRID::dsSTRID(void) */
extern void dsDATA_SetValue_int(dsDATA *data, const int *value);             /* dsDATA::SetValue<int> */
extern void dsDATA_SetValue_tstring(dsDATA *data, const dsTSTRING_flat *value);   /* dsDATA::SetValue<dsTSTRING<char>> */
extern void dsDATA_TYPE_Destroy(const dsDATA_TYPE *type, void *storage);     /* virtual: type->vtable[3](type, storage) */
extern void dsVECTOR_PARAM_LIST_Set_int(dsVECTOR_PARAM_LIST *list, dsSTRID id, const int *value); /* dsVECTOR_PARAM_LIST::Set<int> */
extern void dsVECTOR_PARAM_LIST_Set_tstring(dsVECTOR_PARAM_LIST *list, dsSTRID id, const dsTSTRING_flat *value); /* dsVECTOR_PARAM_LIST::Set<dsTSTRING<char>> */
extern void dsVECTOR_dsPARAM_dtor(dsVECTOR_dsPARAM *v);                      /* dsVECTOR<dsPARAM,8>::~dsVECTOR */
extern void dsSTATIC_PARAM_LIST_1_Add_tstring(dsSTATIC_PARAM_LIST_1 *list, const char *name, const dsTSTRING_flat *value); /* Add<dsTSTRING<char>> */

/* dsSTATIC_PARAM_LIST<2>::dsSTATIC_PARAM_LIST(void) @ 0x823D0788 */
extern void dsSTATIC_PARAM_LIST_2_ctor(dsSTATIC_PARAM_LIST_2 *list);
/* dsSTATIC_PARAM_LIST<2>::~dsSTATIC_PARAM_LIST(void) @ 0x823D0720 */
extern void dsSTATIC_PARAM_LIST_2_dtor(dsSTATIC_PARAM_LIST_2 *list);
/* dsSTATIC_PARAM_LIST<2>::Add<dsTSTRING<char>>(dsSTRID, const dsTSTRING<char>&) @ 0x823D84D8 */
extern void dsSTATIC_PARAM_LIST_2_Add_tstring(dsSTATIC_PARAM_LIST_2 *list, dsSTRID id, const dsTSTRING_flat *value);

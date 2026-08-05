#pragma once
/* Boundary declarations for the HCEX HUD "event modifier" helpers (hcex_update_ammo / hcex_update_heat /
 * hcex_update_compass). Each looks up the ws-engine gfx modifier for a first-person model, builds a
 * ref-counted single-entry parameter list, and fires a named ActionScript event ("asSetAmmo" etc.) on the
 * modifier so the recreated Flash HUD updates.
 *
 * propOBJ_MODIFIER_LIST and the ds_data::REF_TYPE<>/REF_TYPE_DATA<> ref-counted wrapper are ws-engine C++
 * types, treated as extern boundaries: only the touched shape is modeled (layout DB-verified via
 * types_members) and the template "methods" are declared as free functions whose first parameter is the
 * object. The parameter-list container itself is reused from hcex_param_list_boundary.h. */

#include <stdint.h>
#include "hcex_param_list_boundary.h"   /* dsVECTOR_PARAM_LIST, dsSTRID, dsTSTRING_flat, dsSTRID_ctor, dlFree */

/* ws-engine object modifier list (propENT @0 + dsVECTOR<...> listModifiers @40); opaque here. */
typedef struct propOBJ_MODIFIER_LIST propOBJ_MODIFIER_LIST;

/* Canonical template forms of the DB instantiations ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> and
 * ds_data::REF_TYPE_DATA<dsVECTOR_PARAM_LIST> (mirrors the HandlerBase<Host> templatization);
 * the former flat REF_TYPE_*_param_list names are kept as aliases so consumers are unchanged. */
namespace ds_data {

/* ds_data::OBJECT_POLICY — DB-verified (types_enum_values ds_data::OBJECT_POLICY). */
enum OBJECT_POLICY { opRefCount = 0, opExternal = 1, opROExternal = 2 };

/* ds_data::REF_TYPE_DATA<T> — 12 bytes, the heap-allocated ref-counted body. */
template<class T>
struct REF_TYPE_DATA {
    T            *m_pT;         /* 0x00 owned referent */
    int           m_iRefCount;  /* 0x04 */
    OBJECT_POLICY m_op;         /* 0x08 */
};

/* ds_data::REF_TYPE<T> — 4 bytes, the stack-held smart pointer. */
template<class T>
struct REF_TYPE {
    REF_TYPE_DATA<T> *m_pData;  /* 0x00 */
};

} // namespace ds_data

typedef ds_data::REF_TYPE_DATA<dsVECTOR_PARAM_LIST> REF_TYPE_DATA_param_list;
typedef ds_data::REF_TYPE<dsVECTOR_PARAM_LIST>      REF_TYPE_param_list;

extern propOBJ_MODIFIER_LIST *GetModifier(int modelId, int16_t local_player_index);

/* ::operator new(size, file, line) — the debug/placement global allocator the ws build links in. */
extern void *ds_operator_new_dbg(unsigned int size, const char *file, unsigned int line);

extern void REF_TYPE_DATA_param_list_ctor(REF_TYPE_DATA_param_list *self);   /* REF_TYPE_DATA::REF_TYPE_DATA */
extern void REF_TYPE_param_list_dtor(REF_TYPE_param_list *self);             /* REF_TYPE::~REF_TYPE */

extern void dsVECTOR_PARAM_LIST_Add_int(dsVECTOR_PARAM_LIST *list, dsSTRID id, const int *value);      /* Add<int> */
extern void dsVECTOR_PARAM_LIST_Add_double(dsVECTOR_PARAM_LIST *list, dsSTRID id, const double *value); /* Add<double> */

extern void propOBJ_MODIFIER_LIST_EventModifier(propOBJ_MODIFIER_LIST *self, const dsTSTRING_flat *category,
                                                const dsTSTRING_flat *event, const REF_TYPE_param_list *params);

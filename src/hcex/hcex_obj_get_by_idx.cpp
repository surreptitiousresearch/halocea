/* hcex_obj_get_by_idx @0x823EDD80 — return the tracked HCEX_OBJ at a raw vector index, or null when the
 * index is out of range. Unlike hcex_obj_get (which looks up by Blam object id), this indexes hcexObjects
 * directly. hcexObjects is a ws-engine boundary container. */

#include "../headers/hcex/hcex_create_effect_boundary.h"   /* HCEX_OBJ */

typedef struct dsVECTOR_OBJ dsVECTOR_OBJ;          /* dsVECTOR<HCEX_OBJ,8> */

struct hcexObjects_view { void *pData; int nElem; }; // named (anonymous struct has internal linkage)
extern hcexObjects_view hcexObjects;
extern HCEX_OBJ *dsVECTOR_OBJ_index(dsVECTOR_OBJ *v, int i);

HCEX_OBJ *hcex_obj_get_by_idx(int idx)
{
    if ( idx >= 0 && idx < hcexObjects.nElem )
        return dsVECTOR_OBJ_index((dsVECTOR_OBJ *)&hcexObjects, idx);
    return 0;
}

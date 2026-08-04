/* hcex_obj_get @0x823EDDB0 — look up the HCEX_OBJ tracking record for a Blam object id in the sorted
 * hcexObjects vector. Returns the record, or null when the id is not tracked.
 *
 * The decompiler renders the thiscall with spurious extra parameters; the real signature takes only the
 * object id. hcexObjects / its comparator are ws-engine boundary containers (declared, not reversed). */

#include "../headers/hcex/hcex_create_effect_boundary.h"   /* HCEX_OBJ */

typedef struct dsVECTOR_OBJ dsVECTOR_OBJ;          /* dsVECTOR<HCEX_OBJ,8> */
typedef int HCEX_OBJ_CMP;

extern dsVECTOR_OBJ hcexObjects;
extern int       dsVECTOR_OBJ_FindSorted(dsVECTOR_OBJ *v, const int *id, HCEX_OBJ_CMP *cmp);
extern HCEX_OBJ *dsVECTOR_OBJ_index(dsVECTOR_OBJ *v, int i);

HCEX_OBJ *hcex_obj_get(int id)
{
    HCEX_OBJ_CMP cmp = 0;
    int index = dsVECTOR_OBJ_FindSorted(&hcexObjects, &id, &cmp);

    if ( index >= 0 )
        return dsVECTOR_OBJ_index(&hcexObjects, index);
    return 0;
}

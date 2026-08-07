/* hcex_add_model_perm @0x823E09D8 — hcex bridge: append `perm_name` to the object-name list of the
 * `idx`'th region of an HCEX model. `mdl` is typed HCEX_MODEL (DB types_members; the binary
 * prototype passes it as a raw byte pointer): regions@0x34 is dsVECTOR<HCEX_MODEL_REGION,8>,
 * HCEX_MODEL_REGION is 48 bytes with objNames@0x18. ds container ops are ws-engine boundaries. */

#include "../headers/hcex/hcex_ds_boundary.h"
#include "../headers/hcex/HCEX_MODEL.h"

/* DEVIATION: `mdl` was `char *`. The binary's mangled symbol is
 * `?hcex_add_model_perm@@YAXPAXHPBD@Z` — PAX = `void *`, not `char *`. */
void hcex_add_model_perm(void *mdl, int idx, const char *perm_name)
{
    dsTSTRING_flat name; name.pBuffer = nullptr;
    dsTSTRING_UnsafeInit(&name, perm_name, -1, 0);

    /* typed (was raw mdl+52 vector-header walk with a stride-48 region cursor) */
    HCEX_MODEL *model = (HCEX_MODEL *)mdl;
    HCEX_MODEL_REGION *region = &model->regions.pData[idx];
    dsVECTOR_TSTRING_PushBack(&region->objNames, &name);

    if (name.pBuffer->refCount-- == 1)
        dlFree(name.pBuffer);
}

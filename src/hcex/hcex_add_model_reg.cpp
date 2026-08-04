/* hcex_add_model_reg @0x823E57C8 — hcex_enum_model_regions callback: append a new (default) region
 * named `reg_name` to the HCEX_MODEL being built. `mdl` is passed through as a raw void* (mangled
 * signature `void*, const char*`) — the callback context hcex_inst_create sets up (the in-progress
 * HCEX_MODEL's address); the regions field-offset arithmetic the decompiler shows (mdl+52) is just
 * `((HCEX_MODEL*)mdl)->regions`.
 *
 * DEVIATION: the decompiler drops the assignment feeding PushBack's `val` argument (declares the
 * local but never visibly sets it before the call); from context — a default-constructed
 * HCEX_MODEL_REGION immediately preceding the call, destructed via its ordinary dtor right
 * afterward — the pushed value is that local, reconstructed here as a plain PushBack(region) +
 * name assignment (equivalent to the decompiler's manual refcount-adjust-then-release sequence, but
 * expressed through the class's own operator= and destructor). */

#include "../headers/hcex/HCEX_MODEL.h"

void hcex_add_model_reg(void *mdl, const char *reg_name)
{
    HCEX_MODEL *model = (HCEX_MODEL *)mdl;

    dsTSTRING<char> name;
    name.pBuffer = 0;
    name.UnsafeInit(reg_name, -1, 0);

    HCEX_MODEL_REGION region; // default-constructed (objRegId left as-is; set by hcex_inst_create later)
    HCEX_MODEL_REGION &pushed = model->regions.PushBack(region);
    pushed.name = name;
}

// Flattened bridge for hcex_cine_init_boundary.h: free-function form of
// sslSYSTEM::GetGlobalObj() (0x825C9A48, re-sourced in sslSYSTEM__GetGlobalObj.cpp —
// copy-constructs from this->globalObj @0x98). The by-value sret return is materialized by
// constructing the result into `out` (callers pass uninitialized storage, as the sret ABI does).
#include "../../headers/ws/ssl/sslSYSTEM.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/ds_new.h"

void sslSYSTEM_GetGlobalObj(sslOBJ_REF *out, sslSYSTEM *self)
{
    new (out) sslOBJ_REF(self->GetGlobalObj());
}

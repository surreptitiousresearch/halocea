/* ?GetClass@sslOBJ_REF@@QBA?AVsslCLASS_REF@@XZ @0x82524370 */
// Flattened bridge for hcex_cine_init_boundary.h: free-function form of
// sslOBJ_REF::GetClass() const (0x82524370, re-sourced in sslOBJ_REF__GetClass.cpp).
// The by-value sret return is materialized by constructing the result into `out`
// (callers pass uninitialized storage, exactly as the sret ABI does).
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslCLASS_REF.h"
#include "../../headers/ws/ds/ds_new.h"

void sslOBJ_REF_GetClass(sslCLASS_REF *out, sslOBJ_REF *self)
{
    new (out) sslCLASS_REF(self->GetClass());
}

#pragma once
#include "../../ws/ssl/sslOBJ_REF.h"
#include "../../ws/ssl/sslCB_HOST.h"

/* Every cb* trampoline in this batch calls sslOBJ_REF::GetCbHost, a real DB-verified member
 * (0x82523308, `?GetCbHost@sslOBJ_REF@@QBAPAVsslCB_HOST@@XZ`) that predates/postdates whichever
 * pass most recently touched sslOBJ_REF.h and isn't declared there yet. Declared here as a
 * boundary free function (thiscall form) rather than editing that shared, already-completed
 * header — see this corpus's established convention (hcex_ds_boundary.h). */

namespace sslOBJ_REF_bridge {

// 0x82523308 — the callback host of the object this ref points at (const method; `self` is
// passed by value at the real call sites, consistent with sslOBJ_REF's small ref-counted-pointer
// size).
sslCB_HOST *GetCbHost(const sslOBJ_REF *this_);

} // namespace sslOBJ_REF_bridge

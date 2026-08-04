#include "dbgVAR_MANAGER.h"

/* HCEX boundary flat-bridges for the three dbgVAR_MANAGER methods the HCEX bridge layer calls
 * through flattened spellings (haloInit_boundary.h / hcex_dbgvar_boundary.h /
 * hcex_process_frame_boundary.h). The C++ members themselves are already re-sourced:
 *   GetManager     @ 0x82561210 (dbgVAR_MANAGER__GetManager.cpp)
 *   FindFirst      @ 0x8255FB00 (dbgVAR_MANAGER__FindFirst.cpp)
 *   SendVarUpdate  @ 0x8255E020 (dbgVAR_MANAGER__SendVarUpdate.cpp; private — this bridge is a
 *                    friend of dbgVAR_MANAGER for exactly this forward)
 * These shims only adapt the spelling/reference-vs-pointer surface; no logic. */

dbgVAR_MANAGER *dbgVAR_MANAGER_GetManager(void)
{
    return dbgVAR_MANAGER::GetManager();
}

dbgVAR *dbgVAR_MANAGER_FindFirst(dbgVAR_MANAGER *mgr, const dsTSTRING<char> *name)
{
    return mgr->FindFirst(*name);
}

void dbgVAR_MANAGER_SendVarUpdate(dbgVAR_MANAGER *mgr, dbgVAR *var)
{
    mgr->SendVarUpdate(var);
}

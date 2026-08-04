/* hcex_save_dbg_checkpoint @0x823D9AE0 — when the "generate named checkpoint" debug toggle is set, record
 * the supplied name into the "0HCEX/checkpoints/name" string debug variable and trigger a (non-safe) map
 * save. No-op when the toggle is off. dbgVAR_MANAGER / dsTSTRING_flat are ws-engine boundaries. */

#include "../headers/hcex/hcex_dbgvar_boundary.h"   /* dbgVAR*, dbg_hcex_GenChptNamed, dsTSTRING_flat */

extern void main_save_map_nonsafe(void);

extern "C" void hcex_save_dbg_checkpoint(const char *name)
{
    if ( dbg_hcex_GenChptNamed.value )
    {
        dsTSTRING_flat name_str;
        name_str.pBuffer = 0;
        dsTSTRING_UnsafeInit(&name_str, name, -1, 0);

        dsTSTRING_flat var_path;
        var_path.pBuffer = 0;
        dsTSTRING_UnsafeInit(&var_path, "0HCEX/checkpoints/name", -1, 0);

        dbgVAR_MANAGER *manager = dbgVAR_MANAGER_GetManager();
        dbgVAR *var = dbgVAR_MANAGER_FindFirst(manager, &var_path);
        if ( var && var->typeId == 4 )
            dbgVAR_MANAGER_SetDbgVarValue_tstring(manager, var, &name_str);

        if ( --var_path.pBuffer->refCount == 0 )
            dlFree(var_path.pBuffer);
        if ( name_str.pBuffer->refCount-- == 1 )
            dlFree(name_str.pBuffer);

        main_save_map_nonsafe();
    }
}

#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ssl/sslSYSTEM.h"
#include "../../headers/ws/ia/iaIACTOR.h"

// aiCOORD_GROUP::cbHaveMembers @ 0x8322FA98
// ?cbHaveMembers@aiCOORD_GROUP@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: report whether the group currently has any members.
// NOTE: the decompiler models HaveMembers as (this, argc, argv); the DB mangling
// (?HaveMembers@aiCOORD_GROUP@@UAA_NXZ) shows it is a no-argument virtual — the extra argc/argv
// were stale registers. Called with no args here.
void aiCOORD_GROUP::cbHaveMembers(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)argv; (void)caller;

    bool result = HaveMembers();

    dsDATA tmp;
    tmp.type = nullptr;
    tmp.StoreValue<bool>(result);
    retVal.StoreValue(tmp);
}

// aiTASK_DATA_FN::CreateFnPtrByName @ 0x8325FE98
// ?CreateFnPtrByName@aiTASK_DATA_FN@@IAA_NXZ
#include "../../headers/ws/ai/aiTASK_DATA_FN.h"
#include "../../headers/ws/ds/dsFUNC_PTR_SSL.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/ds_new.h"          // tracked operator new(size_t, const char*, int)
#include "../../headers/ws/ssl/sslSYSTEM.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

extern sslSYSTEM *gsSslSystem; // ?gsSslSystem@@3PAVsslSYSTEM@@A — global SSL system instance

// When the target object is only known by name (OBJ_BY_NAME), resolve it through the SSL system and
// build a dsFUNC_PTR_SSL bound to `fnName`. Returns false only when the named object does not exist;
// a nothing-to-do (already OBJ_GIVEN) case returns true.
bool aiTASK_DATA_FN::CreateFnPtrByName()
{
    if (objType == AI_TASK_DATA_OBJ_GIVEN)
        return true;

    sslOBJ_REF ref = gsSslSystem->FindObject(objName);
    if (!ref.pObject)
        return false;

    objType = AI_TASK_DATA_OBJ_GIVEN;
    funcPtr = new ("D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_task_sys.cpp", 0x22F)
        dsFUNC_PTR_SSL((dsDATA)ref, fnName);
    return true;
}

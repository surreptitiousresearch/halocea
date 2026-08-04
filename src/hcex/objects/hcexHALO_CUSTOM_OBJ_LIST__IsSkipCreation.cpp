/* hcexHALO_CUSTOM_OBJ_LIST::IsSkipCreation @0x823D95B8 — true when the instance's class name is
 * exactly "halo_no_model" (used by callers that need the plain skip check without the
 * side-effecting IsInstManagable path). */

#include "../../headers/hcex/hcexHALO_CUSTOM_OBJ_LIST.h"

extern "C" int strcmp(const char *a, const char *b); // boundary — CRT

bool hcexHALO_CUSTOM_OBJ_LIST::IsSkipCreation(scnINST_CREATE_DATA *pInstCD)
{
    return strcmp(pInstCD->nameClass.pBuffer->str, "halo_no_model") == 0;
}

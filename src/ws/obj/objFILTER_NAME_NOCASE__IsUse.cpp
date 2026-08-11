// objFILTER_NAME_NOCASE__IsUse @0x823CE480
#include "../../headers/ws/obj/objFILTER_NAME_NOCASE.h"
#include "../../headers/ws/obj/objOBJ.h"

// 0x823CE48C -- ?IsUse@objFILTER_NAME_NOCASE@@UAAHPAVobjOBJ@@@Z
// Case-insensitive exact-name filter predicate: accept `pObj` iff its name equals this filter's
// name ignoring case. A filter with a null name matches nothing; an object with a null name is
// treated as the empty string.

extern "C" int stricmp(const char *a, const char *b);
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

int objFILTER_NAME_NOCASE::IsUse(objOBJ *pObj)
{
    const char *filterName = this->name;
    const char *objName = pObj->name;

    if (!filterName)
        return 0;
    if (!objName)
        objName = empty_string;

    return stricmp(objName, filterName) == 0;
}

#include "../../headers/ws/obj/objFILTER_NAME.h"
#include "../../headers/ws/obj/objOBJ.h"

// 0x82680440 -- ?IsUse@objFILTER_NAME@@UAAHPAVobjOBJ@@@Z
// Exact-name filter predicate: accept `pObj` iff its name equals this filter's name verbatim.
// A filter with a null name matches nothing; an object with a null name is treated as the empty
// string. The decompiler renders the comparison as an inlined char-by-char strcmp loop; restored
// here as the strcmp call the compiler inlined (cf. objFILTER_NAME_NOCASE::IsUse, which keeps its
// stricmp out of line).

extern "C" int strcmp(const char *a, const char *b);
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

int objFILTER_NAME::IsUse(objOBJ *pObj)
{
    const char *filterName = this->name;
    const char *objName = pObj->name;

    if (!filterName)
        return 0;
    if (!objName)
        objName = empty_string;

    return strcmp(objName, filterName) == 0;
}

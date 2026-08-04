#include "dsDATA_TYPE.h"
#include "ds_boundary.h"
#include "dsTSTRING.h"

// 0x823B4180 — dsDATA_TYPE::GetType<dsTSTRING<char>>()
// Disasm (3 insns): lis r11, dataType@ha ; lwz r3, dataType@l(r11) ; blr — a single load of the
// static singleton pointer dsDATA_TYPE_STORAGE<dsTSTRING<char>>::dataType
// (mangled _dataType___dsDATA_TYPE_STORAGE_V__dsTSTRING_D____2PBVdsDATA_TYPE__B) and return.
//
// Exposed to callers (see src/ws/ssl/sslSCRIPTED_FUNCTION_BASE__ExecCode.cpp) under the wrapper
// name dsDATA_STRING_TYPE(); C++ linkage, matching that file's `extern const dsDATA_TYPE
// *dsDATA_STRING_TYPE();` declaration.
const dsDATA_TYPE *dsDATA_STRING_TYPE()
{
    return dsDATA_TYPE_STORAGE<dsTSTRING<char> >::dataType;
}

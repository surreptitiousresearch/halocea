#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBEST_BASE::GetString @ 0x83178AF8
// ?GetString@aiBEST_BASE@@UBA?AV?$dsTSTRING@D@@ABV2@@Z
//
// Base-class default: ignore the descriptor and return the shared empty string (dsTSTRING default
// ctor adopts the lazily-initialised empty-string singleton).
dsTSTRING<char> aiBEST_BASE::GetString(const dsTSTRING<char> &desc) const
{
    (void)desc;
    return dsTSTRING<char>();
}

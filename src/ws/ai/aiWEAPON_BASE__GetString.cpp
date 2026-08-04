#include "../../headers/ws/ai/aiWEAPON_BASE.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiWEAPON_BASE::GetString @ 0x83178A58
// ?GetString@aiWEAPON_BASE@@UBA?AV?$dsTSTRING@D@@ABV2@@Z
//
// Base-class default: ignore the descriptor and return the shared empty string. The body just
// adopts the lazily-initialised empty-string singleton (dsTSTRING default ctor = UnsafeInitEmpty).
dsTSTRING<char> aiWEAPON_BASE::GetString(const dsTSTRING<char> &desc) const
{
    (void)desc;
    return dsTSTRING<char>();
}

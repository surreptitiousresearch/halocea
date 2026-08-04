#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // _apLog
#include "../../headers/ws/ds/dsTSTRING.h"

// Shared single-NUL empty-string byte @0x8200155A (used to init an empty dsTSTRING).
extern const char byte_8200155A;

// aiBRAIN::implAIGetCurWeaponClassName() @ 0x832B3300
// ?implAIGetCurWeaponClassName@aiBRAIN@@UAA?AV?$dsTSTRING@D@@XZ
//
// Deprecated SSL getter: log a deprecation error naming this instance, then return an empty string.
// (Replacement is GetWeaponTypeBySlotName.)
dsTSTRING<char> aiBRAIN::implAIGetCurWeaponClassName()
{
    _apLog("~AI,Ssl,Error~'%s': function AIGetCurWeaponClassName() is deprecated. Ask ASherstuk for "
           "the replacement(smth like GetWeaponTypeBySlotName)",
           names.nameInst);

    dsTSTRING<char> result;
    result.UnsafeInit(&byte_8200155A, -1, 0);
    return result;
}

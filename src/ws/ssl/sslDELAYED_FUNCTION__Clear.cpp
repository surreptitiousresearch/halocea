/* ?Clear@sslDELAYED_FUNCTION@@QAAXXZ @0x82AF02E0 */
#include "sslDELAYED_FUNCTION.h"
#include "sslOBJ_REF.h"
#include "../ds/dsSTRID.h"

// 0x82AF02E0 — sslDELAYED_FUNCTION::Clear. Reset to an empty/idle delayed call: drop the self and
// caller object refs, clear the function index and pause locks, and null the group tag. (params /
// timeLeft / syncContext are left untouched, matching the binary.)
void sslDELAYED_FUNCTION::Clear()
{
    this->self = sslOBJ_REF();
    this->caller = sslOBJ_REF();
    this->funcIdx = -1;
    this->pauseLocks = 0;
    this->group = dsSTRID();
}

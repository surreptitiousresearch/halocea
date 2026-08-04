#include "sslSOURCE.h"
#include "sslBREAKPOINT.h"
#include "../ds/dsVECTOR.h"

// 0x82A374B8 — sslSOURCE::IsBreakpoint. Scan this source's active breakpoints for the first one on
// `line`; when found, report whether it is scoped to `object_id` (its object_uid). Reproduces the
// binary faithfully: the first line match short-circuits and only object_uid is compared — the
// `class_id` argument is accepted for signature parity but unused.
bool sslSOURCE::IsBreakpoint(int line, int class_id, int object_id)
{
    (void)class_id;

    for (int i = 0; i < this->Breakpoints.nElem; ++i)
    {
        if (this->Breakpoints[i].line == line)
            return object_id == this->Breakpoints[i].object_uid;
    }
    return false;
}

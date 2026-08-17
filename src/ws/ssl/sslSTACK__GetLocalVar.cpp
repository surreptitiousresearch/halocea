/* ?GetLocalVar@sslSTACK@@QAAABVdsDATA@@H@Z @0x82AF1BC0 */
#include "sslSTACK.h"
#include "../ds/dsDATA.h"

// 0x82AF1BC0 — sslSTACK::GetLocalVar. Reference to local `idx`'s value, or a shared function-local
// empty dsDATA returned when `idx` is out of range.
const dsDATA &sslSTACK::GetLocalVar(int idx)
{
    if (idx >= 0 && idx < this->localsStack.nElem)
        return this->localsStack[idx].val;

    static dsDATA emptyValue; // static zero-init → storage=0, type=nullptr
    return emptyValue;
}

#include "dsERROR_HANDLER.h"
#include "dsTSTRING.h"

struct dsDATA; // boundary — ds data object, opaque here

// dsERROR_HANDLER<dsDATA>::IsError @ 0x82AD9530
// True iff instance is the current error owner AND a non-empty error message is set.
template<>
bool dsERROR_HANDLER<dsDATA>::IsError(const dsDATA *instance)
{
    if (GetCurrentInst() != instance)
        return false;
    dsTSTRING<char> errorStr = GetErrorStr();
    if (!errorStr.pBuffer->strLen)
        return false;
    return true;
}

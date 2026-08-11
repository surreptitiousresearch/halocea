#include "../../headers/ws/ap/apCONFIG.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// boundary — variadic logger (declared in several ws boundary headers).
void _apLog(const char *fmt, ...);

// @0x823C4290 — apCONFIG::GetValue<bool>. Resolve `complexKey` to a raw dsDATA, then try a fast
// retrieve conversion; if that fails and the key exists, try an explicit conversion. On any
// failure, log why and return false. The local raw dsDATA's destructor releases its value at each
// return (the decompiler's explicit dsDATA::~dsDATA calls).
template<>
bool apCONFIG::GetValue<bool>(const char *complexKey, bool *val) const
{
    if (!complexKey)
        return false;

    dsDATA raw = GetRawValue(complexKey);

    if (raw.GetValue(*val, DSD_CONV_RETRIEVE) ||
        (!raw.IsNull() && raw.GetValue(*val, DSD_CONV_EXPLICIT)))
    {
        return true;
    }

    const char *reason = raw.IsNull() ? "key doesnt exist in config"
                                       : "explicit conversion failed";
    _apLog("~Cfg,Warning~Couldnt get value <%s>, %s", complexKey, reason);
    return false;
}

#include "psSECTION_RECORD.h"
#include "../ds/dsDATA.h"
// 0x8251760C — referenced copy: shares the interned key id, deep-copies the value (via
// dsDATA::StoreValue), and copies the sort/line bookkeeping fields verbatim.

psSECTION_RECORD::psSECTION_RECORD(const psSECTION_RECORD &that)
{
    keyStrId.id = that.keyStrId.id;
    value.type = nullptr;
    value.StoreValue(that.value);
    sortedIdx = that.sortedIdx;
    lineNmb = that.lineNmb;
}

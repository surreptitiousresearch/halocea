#include "psSECTION_RECORD.h"
#include "../ds/dsSTRID.h"
// 0x82515F8C — default-construct: interned-empty key, empty value.

psSECTION_RECORD::psSECTION_RECORD()
{
    value.type = nullptr;
}

#include "psSECTION_PARSER_OLD.h"
// @0x82A2D368 — default-construct: empty error text (`lineNmb`/`lineStart` are left as whatever the
// caller's storage held, matching the decompiled body — Parse() initializes both before use).

psSECTION_PARSER_OLD::psSECTION_PARSER_OLD()
{
    errorDesc.pBuffer = nullptr;
    errorDesc.UnsafeInitEmpty();
}

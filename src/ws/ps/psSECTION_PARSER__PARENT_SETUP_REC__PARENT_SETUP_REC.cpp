#include "psSECTION_PARSER.h"
// @0x82A2D2D8 — default-construct a deferred parent-setup record: null section handle, empty
// parent-name text.

psSECTION_PARSER::PARENT_SETUP_REC::PARENT_SETUP_REC()
{
    sect.pData = nullptr;
    parent.pBuffer = nullptr;
    parent.UnsafeInitEmpty();
}

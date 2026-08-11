#include "psSECTION_PARSER.h"
// @0x82A2D0F8 — referenced copy of a deferred parent-setup record: copies the section handle
// (bumping its refcount via psSECTION's copy ctor) and shares the parent-name text buffer.

psSECTION_PARSER::PARENT_SETUP_REC::PARENT_SETUP_REC(const PARENT_SETUP_REC &that)
    : sect(that.sect)
{
    parent.pBuffer = that.parent.pBuffer;
    ++parent.pBuffer->refCount;
}

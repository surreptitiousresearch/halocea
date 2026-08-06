#include "psSECTION_PARSER.h"
#include "psSECTION_DATA.h"
// Debug allocator free (matches the dsTSTRING buffer's manual refcount teardown). boundary.
extern "C" void dlFree(void *ptr);
// 0x8251A9D4 — release the parent-name text buffer and, if held, the section body reference.

psSECTION_PARSER::PARENT_SETUP_REC::~PARENT_SETUP_REC()
{
    if (--parent.pBuffer->refCount == 0)
        dlFree(parent.pBuffer);
    if (sect.pData)
        sect.pData->Release();
}

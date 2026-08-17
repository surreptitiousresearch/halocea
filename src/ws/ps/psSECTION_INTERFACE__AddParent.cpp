/* ?AddParent@psSECTION_INTERFACE@@QAAXVpsSECTION@@@Z @0x8251D700 */
#include "psSECTION_INTERFACE.h"
#include "psSECTION.h"
#include "psSECTION_DATA.h"
#include "psSECTION_BRANCH.h"
// 0x8251D700 — add `parent` to this section's parent list. Creates an empty body first if the
// handle is null, forwards a freshly referenced copy of the handle to the body, then releases the
// incoming by-value handle. `parent` is a by-value psSECTION the callee owns and releases.

void psSECTION_INTERFACE::AddParent(psSECTION parent)
{
    if (parent.pData) {
        if (!this->pData)
            this->CreateEmpty();

        psSECTION handle;
        handle.pData = parent.pData;
        if (handle.pData) {
            ++handle.pData->refCount;
            ++handle.pData->pBranch->refCount;
        }
        this->pData->AddParent(handle);

        if (parent.pData)
            parent.pData->Release();
    }
}

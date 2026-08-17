/* ?SetIgnoreIndirectParent@psSECTION_INTERFACE@@QAAXH@Z @0x8251A540 */
#include "psSECTION_INTERFACE.h"
#include "psSECTION_DATA.h"
// 0x8251A554 — set (bit 1, 0x02) or clear the "ignore indirectly-inherited parent lookups" flag in
// `state`. When setting it on a null handle, an empty body is allocated first; clearing it on a null
// handle is a no-op.

void psSECTION_INTERFACE::SetIgnoreIndirectParent(int b)
{
    if (!this->pData) {
        if (!b)
            return;
        CreateEmpty();
    }

    psSECTION_DATA *data = this->pData;
    data->state = b ? (data->state | 0x02) : (data->state & ~0x02u);
}

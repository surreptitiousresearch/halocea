/* ?SetToDsData@sslSTACK_DATA@@QAAAAV1@ABVdsDATA@@@Z @0x82AF0B88 */
#include "sslSTACK_DATA.h"
#include "../ds/dsDATA.h"

// Global placement copy-construct helper (New<T>(dst, src)); body not reversed here.
template<class T> T *New(T *dst, const T *src); // boundary

// 0x82AF0B88 — sslSTACK_DATA::SetToDsData. Copy-constructs a dsDATA into the payload region.
sslSTACK_DATA &sslSTACK_DATA::SetToDsData(const dsDATA &val)
{
    Cleanup();
    type = TYPE_DS_DATA;
    New<dsDATA>((dsDATA *)&data, &val); // in-place copy into the 12-byte payload
    return *this;
}

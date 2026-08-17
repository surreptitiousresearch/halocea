/* ?SetToDsDataMemberRef@sslSTACK_DATA@@QAAAAV1@ABVdsDATA@@VdsSTRID@@@Z @0x82AF0BD8 */
#include "sslSTACK_DATA.h"
#include "../ds/dsDATA.h"
#include "../ds/dsSTRID.h"

// Global placement copy-construct helper (New<T>(dst, src)); body not reversed here.
template<class T> T *New(T *dst, const T *src); // boundary

// 0x82AF0BD8 — sslSTACK_DATA::SetToDsDataMemberRef. Copy-constructs the dsDATA into the payload and
// records the interned member id. `id` is a by-value dsSTRID (a single const char*); the decompiler
// renders it through a spilled slot but it is one register argument.
sslSTACK_DATA &sslSTACK_DATA::SetToDsDataMemberRef(const dsDATA &val, dsSTRID id)
{
    Cleanup();
    type = TYPE_DSDATA_MEMBER_REF;
    New<dsDATA>((dsDATA *)&dsDataMemberRef, &val); // in-place copy into the 8-byte dsDATA sub-slot
    dsDataMemberRef.id = id.CStr();
    return *this;
}

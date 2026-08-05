#include "sslSTACK_DATA.h"
#include "sslERROR.h"
#include "../ds/dsDATA.h"
#include "../ds/dsSTRID.h"
#include "../ds/ds_assert_boundary.h"

// 0x82AF0EF0 — sslSTACK_DATA::GetAsDsDataMemberRef. Read out a TYPE_DSDATA_MEMBER_REF payload: copy
// the referenced in-place dsDATA (the first 8 bytes of the member-ref arm) into `val` and its
// interned member id into `id`; returns a no-error sslERROR by value (sret). The decompiler swaps
// this/result (r3 = return slot sslERROR*, r4 = this sslSTACK_DATA*).
sslERROR sslSTACK_DATA::GetAsDsDataMemberRef(dsDATA &val, dsSTRID &id) const
{
    // The member-ref arm's leading 8 bytes hold a constructed dsDATA (storage@0, type@4).
    val.StoreValue(*reinterpret_cast<const dsDATA *>(&this->dsDataMemberRef));

    if (!IGNORE_STRONG_ASSERT)
    {
        dsSTRID check(this->dsDataMemberRef.id, 1);
        if (check.CStr() != this->dsDataMemberRef.id)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                "dsSTRID(dsDataMemberRef.id).CStr() == dsDataMemberRef.id",
                "D:\\Projects\\code\\common\\src.sys\\ssl\\ssl_stack.cpp",
                437,
                empty_string);
    }

    id.id = this->dsDataMemberRef.id;
    return sslERROR();
}

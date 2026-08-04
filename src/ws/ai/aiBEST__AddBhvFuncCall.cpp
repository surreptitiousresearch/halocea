#include "../../headers/ws/ai/aiBEST.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern int IGNORE_STRONG_ASSERT;    // _IGNORE_STRONG_ASSERT
extern unsigned char byte_8200155A; // shared empty-string / assert-info byte @0x8200155A

// aiBEST::AddBhvFuncCall @ 0x832974E8
// ?AddBhvFuncCall@aiBEST@@UAA_NABV?$dsTSTRING@D@@0@Z
//
// Offer the SSL ordered function call (identified by behaviour prefix + function name) to every live
// changeable behaviour, top to bottom. Finished behaviours are skipped. Returns true if at least one
// behaviour accepted the call.
bool aiBEST::AddBhvFuncCall(const dsTSTRING<char> &bhvPrefix, const dsTSTRING<char> &nameFunc)
{
    bool accepted = false;
    for (int i = arrChangeable.nEntry - 1; i >= 0; --i) {
        if (!IGNORE_STRONG_ASSERT && (i >= 10 || i >= arrChangeable.nEntry))
            STRONG_ASSERT_DUMMY::Crash(nullptr, "IsValidIdx(idx)",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_const_list.h", 166, byte_8200155A);

        bhvBEHAV *b = arrChangeable.list[i];
        if (b && (b->state.val & BHV_ST_FINISHED) == 0 && b->AddSSLFuncCall(bhvPrefix, nameFunc))
            accepted = true;
    }
    return accepted;
}

#pragma once
#include "dsVECTOR.h"
#include "dsTSTRING.h"
// ws-engine ds: a compiled boolean condition over a set of interned flag keys (K), evaluated
// against a live flag set (F). Parsed from a text expression (strCondition) into a postfix-ish
// list of CMD tokens. DB-verified layout (types_members dsFLAGS_CONDITION<dsSTRID,dsSTRID_FLAGS>):
//   commands@0 (dsVECTOR<CMD,8>, 20), strCondition@20 (dsTSTRING<char>, 4) — size 24.

template<class K, class F>
struct dsFLAGS_CONDITION {
    // dsFLAGS_CONDITION<...>::CMD — one condition token. DB-verified (dsSTRID instantiation):
    // type@0 (u8), value@4 (K) — size 8. `type` selects operator (and/or/not/push-key/...).
    struct CMD {
        unsigned char type;  // 0x00 operator/opcode
        K             value; // 0x04 key operand (for push-key tokens)
    };

    dsVECTOR<CMD, 8>  commands;     // 0x00 compiled token list
    dsTSTRING<char>   strCondition; // 0x14 source expression text

    // ?Execute@?$dsFLAGS_CONDITION@VdsSTRID@@UdsSTRID_FLAGS@@@@QBA_NABUdsSTRID_FLAGS@@@Z @ 0x831D1C50
    // — evaluate the compiled condition against `flags`; an empty condition is vacuously true.
    // boundary — body external to this batch.
    bool Execute(const F &flags) const;
};

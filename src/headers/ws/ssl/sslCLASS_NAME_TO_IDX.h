#pragma once
#include "../ds/dsSTRID.h"
// ssl subsystem: sslCLASS::NAME_TO_IDX — a (name -> element index) map entry, the element type of
// dsVECTOR<sslCLASS::NAME_TO_IDX,8> sslCLASS::nameToElemIdx. DB-verified layout
// (types_members sslCLASS::NAME_TO_IDX): name@0 (dsSTRID), idx@4 (int) — size 8.
// Completes the forward declaration `namespace ssl_class { struct NAME_TO_IDX; }` in sslCLASS.h.

namespace ssl_class {
    struct NAME_TO_IDX {
        dsSTRID name; // 0x00 interned element name
        int     idx;  // 0x04 index into sslCLASS::elements
    };
}

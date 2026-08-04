#pragma once
#include "dsVECTOR.h"
#include "dsSTRID.h"
// ws-engine ds: a growable list of string-id "flags" (property/event names as interned dsSTRID).
// DB-verified layout (types_members dsSTRID_FLAGS) — size 20 (0x14): a single dsVECTOR<dsSTRID,8>.

typedef struct dsSTRID_FLAGS {
    dsVECTOR<dsSTRID, 8> propList; // 0x00 flag/property name list

    // Construct an empty flag set. ds boundary.
    dsSTRID_FLAGS();
    // Construct a single-element flag set containing `id`. ds boundary (body external to this batch).
    explicit dsSTRID_FLAGS(const dsSTRID &id);

    // Replace this set's contents with `other`'s (clears then copies). ds boundary.
    void Set(const dsSTRID_FLAGS &other);

    // Add (`value`!=0) or remove (`value`==0) the interned name `id`, keeping propList sorted. boundary.
    void Set(const dsSTRID &id, bool value);
} dsSTRID_FLAGS;

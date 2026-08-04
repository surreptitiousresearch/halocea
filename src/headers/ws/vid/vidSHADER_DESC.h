#pragma once
// ws-engine vid: per-pass shader descriptor entry used by vidMTL::sdrDescList.
// DB-verified layout (types_members vidSHADER_DESC): passID@0, sdrDesc@2 — size 4
// (1 byte padding between the two fields).

typedef struct vidSHADER_DESC {
    unsigned char  passID;  // 0x00
    unsigned char _pad0[1]; /* db-verified padding */
    unsigned short sdrDesc; // 0x02
} vidSHADER_DESC;

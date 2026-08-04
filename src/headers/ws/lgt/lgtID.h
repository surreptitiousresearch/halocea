#pragma once
// ws-engine lgt: 4-byte light identifier. DB-verified layout (types_members lgtID):
//   typeId@0 (unsigned __int8), id@2 (__int16) — size 4.
// The two bytes at offset 1 and (the high byte) are padding in the DB layout; the packed
// { typeId, id } pair is what rendSR_DATA lamp records encode (see rendSR_DATA::IsLampVisible).

typedef struct lgtID {
    unsigned char typeId; // 0x00 light-type discriminator
    unsigned char _pad01; // 0x01
    short         id;      // 0x02 per-type light index
} lgtID;

#pragma once
// ws-engine rend: one packed static-lamp-render descriptor slot, the element type of
// dsVECTOR<rendSR_LAMP_DATA,8> (a static-lighting lamp table). DB-verified layout
// (types_members rendSR_LAMP_DATA): data@0 (unsigned int) — size 4.
// The bit-packed sub-fields of `data` are left opaque here (no consumer of this batch reads
// individual bits) — kept as a raw packed word, faithful to the DB's single-member layout.

typedef struct rendSR_LAMP_DATA {
    unsigned int data; // 0x00 packed lamp descriptor bits
} rendSR_LAMP_DATA;

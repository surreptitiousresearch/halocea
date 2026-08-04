#pragma once
// ws-engine ds: fixed-size array of machine words (a "bit array" of NELEM 32-bit cells).
// DB-verified layout (types_members dsFIXED_BIT_ARRAY<4>): array@0 (unsigned int[NELEM]) — size 4*NELEM.
// Template param NELEM = number of 32-bit cells.

template<int NELEM>
struct dsFIXED_BIT_ARRAY {
    unsigned int array[NELEM]; // 0x00

    // 0x826C1560 (dsFIXED_BIT_ARRAY<4>) — bounds-checked read of cell `index`.
    unsigned int GetAt(unsigned int index) const;

    // Inequality over the whole cell array (nonzero iff any cell differs); body not decompiled here.
    bool operator!=(const dsFIXED_BIT_ARRAY<NELEM> &other) const; // boundary

    // Equality over the whole cell array (true iff every cell matches); referenced by
    // propHALO_OBJ_DESC::SetSslInitFlagsForHier. Body not decompiled here. // boundary
    bool operator==(const dsFIXED_BIT_ARRAY<NELEM> &other) const; // boundary
};

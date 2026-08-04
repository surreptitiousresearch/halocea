#pragma once
// ws-engine ds: low-level bit-array primitive operations over a caller-owned word array.
// Static helpers shared by the dsBIT_ARRAY / dsFIXED_BIT_ARRAY wrappers. The array holds
// `n_elem` 32-bit cells; bit `i` lives in word Bit2Idx(i) = i>>5 with mask 1<<(i&31).

typedef struct dsBIT_ARRAY_IMPL {
    // 0x8253BFC0 — set bit `i` to 1; asserts 0 <= i && (i>>5) < n_elem.
    static void SetYes(int i, unsigned int *array, int n_elem);

    // 0x8253C060 — clear bit `i` to 0; asserts 0 <= i && (i>>5) < n_elem.
    static void SetNo(int i, unsigned int *array, int n_elem);

    // Test bit `i`; asserts 0 <= i && (i>>5) < n_elem. boundary (reversed in ds).
    static bool Is(int i, const unsigned int *array, int n_elem);
} dsBIT_ARRAY_IMPL;

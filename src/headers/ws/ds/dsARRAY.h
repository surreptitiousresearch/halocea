#pragma once
// ws-engine ds: fixed-size inline array wrapper. DB-verified layout
// (types_members ds::ARRAY<unsigned short,5> / ds::ARRAY<short,3> / ds::ARRAY<HALO_SOUND_TAG
// const *,15>): a single member `list` of N elements at offset 0 — size = N * sizeof(T).
// No growth and no count of its own; the caller tracks how many slots are live. operator[] is
// inlined at every call site in the binary (never an out-of-line symbol), so it is modelled here
// as a plain bounds-unchecked index.
// Provenance merged from the duplicate ws/ds/ds_ARRAY.h, deleted 2026-08-06: it defined the SAME
// ds::ARRAY at file scope (an odr_dup), and the linkdup drain removed its last consumer.

namespace ds {

template<class T, int N>
struct ARRAY {
    T list[N]; // 0x00
    T &operator[](int i) { return list[i]; }
    const T &operator[](int i) const { return list[i]; }
};

} // namespace ds

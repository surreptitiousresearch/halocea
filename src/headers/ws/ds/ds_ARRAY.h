#pragma once
// ws-engine ds: fixed-capacity inline array (no growth, no count tracking of its own — the
// caller tracks how many slots are live). DB-verified layout (types_members
// ds::ARRAY<HALO_SOUND_TAG const *,15>): list@0 is a plain T[N] — size N*sizeof(T), no other
// fields. operator[] is inlined at every call site in the binary (never an out-of-line
// function symbol), so it is modeled here as a plain bounds-unchecked index into `list`.

namespace ds {

template<class T, int N>
struct ARRAY {
    T list[N]; // 0x00

    T &operator[](int idx) { return list[idx]; }
    const T &operator[](int idx) const { return list[idx]; }
};

} // namespace ds

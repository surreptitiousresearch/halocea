#pragma once
// ws-engine ds: fixed-size inline array wrapper. DB-verified layout
// (types_members ds::ARRAY<unsigned short,5> / ds::ARRAY<short,3>): a single
// member `list` of N elements at offset 0 — size = N * sizeof(T).

namespace ds {

template<class T, int N>
struct ARRAY {
    T list[N]; // 0x00
    T &operator[](int i) { return list[i]; }
    const T &operator[](int i) const { return list[i]; }
};

} // namespace ds

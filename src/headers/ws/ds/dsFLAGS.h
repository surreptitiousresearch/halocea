#pragma once
// ws-engine ds: type-safe bitflag wrapper around a single integer.
// DB-verified layout (types_members dsFLAGS<enum FIO_OPEN,int>): val@0 — size 4.
// Template params: E = the enum whose members name the individual bits; T = the
// underlying storage integer. MSVC mangles the instantiation as
// V?$dsFLAGS@W4FIO_OPEN@@H@@ == dsFLAGS<FIO_OPEN, int>.

template<class E, class T>
struct dsFLAGS {
    T val; // 0x00 packed bitmask over E's members

    // Convenience accessors (bodies external to this batch). boundary.
    bool Test(E bit) const;      // boundary
    void Set(E bit);             // boundary
    void Clear(E bit);           // boundary
};

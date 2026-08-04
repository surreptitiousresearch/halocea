#pragma once
#include "../ds/dsDATA.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
// ws-engine ps: a 4-byte ref-counted handle to an array of dsDATA elements.
// DB-verified layout (types_members psARRAY): pData@0 (psARRAY_DATA*) — size 4.

struct psARRAY_DATA; // ref-counted backing store (full layout in psARRAY_DATA.h)

typedef struct psARRAY {
    psARRAY_DATA *pData; // 0x00 shared array data

    psARRAY();                                     // boundary — default construct (null handle)
    // 0x827B00A0 — build an array handle from a boxed dsDATA value: retrieve directly if it already
    // holds a psARRAY; otherwise allocate a fresh backing store and wrap `value` as a 1-element array;
    // a null (empty) `value` is a contract violation (STRONG_ASSERT crashes unless suppressed).
    explicit psARRAY(const dsDATA &value);
    ~psARRAY();                                    // boundary

    // 0x827AE838 — element count (0 when the handle is null). const (QBA).
    int Length() const;

    // 0x827AEA80 — box this array as a dsDATA value (empty dsDATA when the handle is null).
    dsDATA CreateObject();

    // 0x827AF500 — element `idx` coerced to float, or `def` when absent/uncoercible. const (QBA).
    float GetFloat(int idx, float def) const;

    // 0x827AF560 (?GetStr@psARRAY@@QBA?AV?$dsTSTRING@D@@HV2@@Z) — element `idx` coerced to a
    // string, or `def` when absent/uncoercible. Returns dsTSTRING<char> by value (sret). `def` is
    // passed by value (invisible reference, per dsTSTRING's non-trivial copy ctor/dtor). Body not
    // reversed in this batch. // boundary
    dsTSTRING<char> GetStr(int idx, dsTSTRING<char> def) const;

    // 0x827AF3C0 — element `idx` as a dsDATA (empty when out of range/null). const (QBA).
    dsDATA GetElement(int idx) const;

    // 0x827AFC78 — append `data` to the array, lazily allocating the backing store on first use.
    void PushBack(const dsDATA &data);

    // 0x827B01C0 — replace this array's contents with a copy of `data`'s elements (lazily
    // allocating the backing store on first use).
    void SetElements(const dsVECTOR<dsDATA, 8> &data);

    psARRAY &operator=(const psARRAY &other);      // boundary
} psARRAY;

#pragma once
// ws-engine ps: a 4-byte section handle — a single pointer to a ref-counted section body
// (psSECTION_DATA). DB-verified layout (types_members psSECTION): pData@0 (psSECTION_DATA*) — size 4.
// The `handle` alias preserves the legacy unsigned-int spelling used by the dsVECTOR<psSECTION,8>
// element-copy code; both names name the same 4-byte slot.

struct psSECTION_DATA; // ref-counted section body (full layout in psSECTION_DATA.h)
struct dsDATA;         // boxed value (full layout in ../ds/dsDATA.h)

typedef struct psSECTION {
    // The legacy dsVECTOR<psSECTION,8> element-copy code treats this same
    // 4-byte slot as an unsigned int handle.
    // Anonymous union: `pData` is the DB member name; `handle` is the legacy unsigned-int spelling
    // used by the dsVECTOR<psSECTION,8> element-copy code. Same 4-byte slot at 0x00 (layout-neutral).
    union {
        psSECTION_DATA *pData;  // 0x00 DB member name — ref-counted section body
        unsigned int    handle; // 0x00 legacy alias
    };

    psSECTION();                                   // boundary — default construct (empty section)
    // 0x8251ABA0 — extract a referenced section handle from a boxed dsDATA value (null unless
    // `value` currently holds a psSECTION; GetValue bumps the section's refcount on retrieval).
    explicit psSECTION(const dsDATA &value);
    psSECTION(const psSECTION &other);             // boundary — referenced copy (bumps refcount)
    ~psSECTION();                                  // boundary
    psSECTION &operator=(const psSECTION &other); // boundary
} psSECTION;

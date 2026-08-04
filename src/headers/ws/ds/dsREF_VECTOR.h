#pragma once
#include "dsDATA.h"
#include "dsSTRID.h"
// ws-engine ds: dsREF_VECTOR itself is a large container class external to this batch —
// only its nested SortCmp comparator functor (used to sort a dsVECTOR<dsDATA,8> of referenced
// values by invoking a named SSL/script function) is modeled here, as required by
// dsVECTOR<dsDATA,8>::Sort/HeapSort/Sisp.

class dsREF_VECTOR { // boundary — external to this batch
public:
    // 0x8261CD38 (?Push@dsREF_VECTOR@@QAAXABVdsDATA@@@Z) — append a copy of `d` to this vector.
    // Body external to this batch. // boundary
    void Push(const dsDATA *d);

    // DB-verified layout (types_members dsREF_VECTOR::SortCmp): m_obj@0 (dsDATA), m_sFunc@8
    // (dsSTRID) — size 12.
    struct SortCmp {
        dsDATA  m_obj;    // 0x00 the object value whose named method compares two elements
        dsSTRID m_sFunc;  // 0x08 interned name of the comparison function to invoke on m_obj

        // 0x8261AE78 — construct from the comparison object and function name.
        SortCmp(dsDATA &obj, dsSTRID func); // boundary
        // 0x8261AD10 — release m_obj's held value (dsDATA::~dsDATA).
        ~SortCmp(); // boundary

        // 0x8261AEC0 — order two dsDATA values by dispatching m_sFunc on m_obj; returns <0, 0, >0
        // like a normal three-way comparator. Not reversed in this batch (crosses into the
        // sslSCRIPTED_FUNCTION call machinery).
        int cmp(const dsDATA &left, const dsDATA &right) const; // boundary
    };
};

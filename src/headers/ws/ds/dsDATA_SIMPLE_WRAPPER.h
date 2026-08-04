#pragma once
#include "dsDATA.h"
// ws-engine ds: a thin typed wrapper adapting a concrete C++ value T into (and, for some T, out
// of) a generic dsDATA. `operator dsDATA()` boxes `val` into a type-erased dsDATA using T's
// dsDATA_TYPE descriptor (via dsDATA::StoreValue<T>); the (dsDATA, int*) constructor is the
// inverse -- unbox a dsDATA back into T (retrieval), reversed here for T=dsTSTRING<char>.
// DB-verified layout (types_members dsDATA_SIMPLE_WRAPPER<T>): val@0, size == sizeof(T).

template<class T>
struct dsDATA_SIMPLE_WRAPPER {
    T val; // 0x00

    // 0x82504528 (T = dsTSTRING<char>) — retrieve a T out of `data` (DSD_CONV_RETRIEVE) into
    // `val`. On failure, if `isOk` is non-null and still true, clear it to record the failure.
    // Reversed only for T = dsTSTRING<char> in this batch; declared generically here (other
    // instantiations, if any exist, are not decompiled).
    dsDATA_SIMPLE_WRAPPER(const dsDATA &data, int *isOk);

    // boundary (usage-inferred; 2026-07-31) — construct by wrapping a concrete T value (the box
    // direction's value ctor; DB-confirmed "ctor from value"). Body external to this batch;
    // used by sslSCRIPTED_FUNCTION_BASE::ExecCode (SSL_BC_SUBREF_TO_CONST_NAME).
    dsDATA_SIMPLE_WRAPPER(const T &value);

    // Convert `val` to a generic dsDATA (adopts T's dsDATA_TYPE descriptor via StoreValue<T>).
    // Reversed for T = dsTSTRING<char>, int, bool, float in this batch (each an explicit
    // specialization in its own .cpp per this corpus's template convention).
    // ABI note: dsDATA is 8 bytes, so this const method returns via a hidden sret pointer; see
    // each .cpp's CAVEAT for how the decompiler mistypes the hidden return/this pointers.
    operator dsDATA() const;
};

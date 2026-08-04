#pragma once
#include "../ds/dsTSTRING.h"
// ws-engine debug-variable base classes (ap_whiteboard.cpp). BOUNDARY -- not decompiled in
// this batch; only enough surface is modeled to type wb::dbgVARWrapper::GetValue and
// wb::RegisterPage, which construct/destroy a dbgVAR_SIMPLE<bool,1> through this hierarchy.
// DB-verified layout (types_members dbgVAR / dbgVAR_IMPL<bool,1> / dbgVAR_SIMPLE<bool,1> /
// dbgVAR_vtbl / dbgVAR_IMPL<bool,1>_vtbl):
//   dbgVAR:              __vftable@0, name@4 (dsTSTRING<char>), typeId@8 (const int) -- size 12.
//   dbgVAR_IMPL<bool,1>: dbgVAR (0, size 12) + value@12, prevValue@13, defaultValue@14 (bool)
//                        -- size 15, padded to 16.
//   dbgVAR_SIMPLE<bool,1>: dbgVAR_IMPL<bool,1> (0, size 16), no added fields, own vtable
//                        (`dbgVAR_SIMPLE<bool,1>::`vftable'') -- size 16.
//   vtbl (both dbgVAR_vtbl and dbgVAR_IMPL<bool,1>_vtbl, same 4 slots): dtr@0, GetAsPsData@4,
//   ReadPs@8, Update@12.

struct dsDATA; // ws-engine ds — boundary, only used by pointer/reference here
struct psSECTION; // ws-engine ps — boundary, only used by value here

typedef struct dbgVAR_vtbl {
    // types_members reports this slot's pointee as a 1-arg `void(dbgVAR*)`, but the call site
    // at 0x8269DB00 (dbgVARWrapper::~dbgVARWrapper) loads r4=1 before the indirect branch --
    // a real 2nd argument, matching the standard vector-deleting-destructor ABI (flag bit 0 =
    // also free the object's storage). Modeled with the 2nd arg to match observed disasm.
    void     (__fastcall *dtr)(struct dbgVAR *self, int freeMemory);
    dsDATA  *(__fastcall *GetAsPsData)(struct dbgVAR *self, dsDATA *result);
    void     (__fastcall *ReadPs)(struct dbgVAR *self, psSECTION section);
    void     (__fastcall *Update)(struct dbgVAR *self);
} dbgVAR_vtbl;

typedef struct dbgVAR {
    dbgVAR_vtbl     *__vftable; // 0x00
    dsTSTRING<char>  name;      // 0x04
    const int        typeId;    // 0x08

    static dbgVAR_vtbl vftable; // `dbgVAR::`vftable''  -- boundary, not decompiled

    // ??0dbgVAR@@QAA@ABV?$dsTSTRING@D@@H@Z @ 0x82561658 -- shares `_name`'s buffer (manual
    // refcount bump, matching dsTSTRING<char>'s copy-share convention) and sets typeId. Then
    // looks itself up by name in the process-wide dbgVAR_MANAGER: if a prior variable of the same
    // name but a DIFFERENT typeId already exists, that's a registration bug -- STRONG_ASSERT-crashes
    // (unless IGNORE_STRONG_ASSERT is set). Finally registers `this` via dbgVAR_MANAGER::AddVar.
    dbgVAR(const dsTSTRING<char> &_name, int _typeId);
} dbgVAR;

// Concrete typed value + prev/default shadow, e.g. dbgVAR_IMPL<bool,1>. Constructor is a
// boundary (not decompiled as a standalone .cpp) -- confirmed via disasm at 0x8269DE10
// (dbgVAR_IMPL<bool,1>::dbgVAR_IMPL(dsTSTRING<char> const&, bool const&)) that it takes the
// display name and initial/default value by const reference and sets __vftable to this
// level's own vtable.
template<class T, int N>
struct dbgVAR_IMPL : dbgVAR {
    T value;        // 0x0C current value
    T prevValue;    // 0x0D value as of the last frame/poll
    T defaultValue; // 0x0E value RegisterPage was called with

    dbgVAR_IMPL(const dsTSTRING<char> &displayName, const T &initialValue); // boundary

    // e.g. 0x823C2364 (dbgVAR_IMPL<dsTSTRING<char>,4>::GetValue, called from dbgVAR_STRING::CStr) --
    // const accessor for `value`. QBA mangle (public const), returns by const reference.
    const T &GetValue() const;
};

// dbgVAR_SIMPLE<T,N> adds no fields over dbgVAR_IMPL<T,N> -- it exists purely to install its
// own vtable (overriding dtr/GetAsPsData/ReadPs/Update) over an otherwise-identical layout.
template<class T, int N>
struct dbgVAR_SIMPLE : dbgVAR_IMPL<T, N> {
    static dbgVAR_vtbl vftable; // `dbgVAR_SIMPLE<T,N>::`vftable''  -- boundary, not decompiled
};

#pragma once
#include "../ds/dsTSTRING.h" // SetDbgInfo takes dsTSTRING<char> by value — needs the full type (2026-07-31)
// ssl subsystem: ref-counted reference to a script class descriptor. DB-verified layout
// (types_members sslCLASS_REF): pClass@0 — size 4. The copy-ctor/dtor adjust the class
// refcount. Method bodies that dereference pClass live in the .cpp (which includes
// sslCLASS.h) so this header only forward-declares sslCLASS.

struct sslCLASS;   // ssl script class descriptor (full definition in sslCLASS.h)
struct sslSYSTEM;  // boundary — owning script system
struct sslOBJ_REF; // ssl script object reference (full definition in sslOBJ_REF.h)
struct dsDATA;     // ds type-erased value (full definition in ds/dsDATA.h)
struct sslERROR;   // ssl error/result value (full definition in sslERROR.h)

// Global-form native callback signature (typedef'd so the method declaration below
// does not read as a function-pointer data member).
typedef void (*sslCB_GLOBAL_FN)(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal,
                                sslOBJ_REF caller);

struct sslCB_HOST; // native callback host object (full definition in sslCB_HOST.h)

// Member-form native callback signature: a pointer-to-member of the host object, invoked with the
// argument count, the arg/return dsDATA buffers, and the calling object ref.
typedef void (sslCB_HOST::*sslCB_MEMBER_FN)(int argc, dsDATA *argv, dsDATA &retVal,
                                            sslOBJ_REF caller);

typedef struct sslCLASS_REF {
    sslCLASS *pClass; // 0x00

    sslCLASS_REF();                          // 0x82523188 — empty ref (pClass = null)
    sslCLASS_REF(const sslCLASS_REF &other); // 0x825238A0 — ref-counted copy
    ~sslCLASS_REF();                         // 0x825234E0 — ref-counted release
    sslCLASS_REF &operator=(const sslCLASS_REF &other); // 0x82523910 — ref-counted assign

    // 0x8270D8F0 — register a native global-form callback (`cb`) as a script-callable function
    // named `name` on the referenced class; `msg` is an (unused-here) doc/help string, `outIdx`
    // optionally receives the new function element's index (null accepted). Returns an sslERROR
    // by value (sret). Reversed only for its call-site shape (hcex_library_init); body is a
    // boundary outside this re-source. QAA.
    sslERROR AddCbFunc(const char *name, sslCB_GLOBAL_FN cb,
                        const char *msg, int *outIdx); // boundary

    // 0x82524DD0 — member-callback overload of AddCbFunc: register the pointer-to-member native
    // callback `cbFunc` under the function declaration `decl` on the referenced class. `errHint` is a
    // source-location string logged with any parse/registration error; `idx` optionally receives the
    // new function element's index. Returns an sslERROR by value (sret); an SSL_ERR_GENERIC error
    // when the ref is empty. QAA. Parses `decl` with a scratch sslLEX and hands off to
    // sslPARSER::AddCbFuncToClass (a re-source boundary, one level past this method).
    sslERROR AddCbFunc(const char *decl, sslCB_MEMBER_FN cbFunc, const char *errHint, int *idx);

    // 0x82525C80 (?AddScript@sslCLASS_REF@@QAA?AVsslERROR@@PBD0@Z) — compile `code` into this class,
    // tagging diagnostics with `dbgHint`. boundary.
    sslERROR AddScript(const char *code, const char *dbgHint);
    // 0x82524860 (?SetDbgInfo@sslCLASS_REF@@QAAXV?$dsTSTRING@D@@@Z) — attach a debug name to the
    // class. boundary.
    void SetDbgInfo(dsTSTRING<char> dbgName);

    // 0x825231E8 — the owning script system of the referenced class, or null if empty. const (QBA).
    sslSYSTEM *GetSslSystem() const;
    // 0x82523200 — uid of the referenced class, or -1 if empty. const (QBA).
    int GetUID() const;
    // 0x82523540 — retarget pClass to `pCls` with ref-count fixup (adds a ref to the new class,
    // releases and destroys the old one when its count hits zero). QAA.
    void _SetClassPtr(sslCLASS *pCls);

    // 0x82523980 — the parent (base) class reference of the referenced class; an empty ref when
    // this ref is empty or has no parent. Returns sslCLASS_REF by value (sret). const (QBA).
    sslCLASS_REF GetParent() const;

    // 0x82525690 (?RegisterEvent@sslCLASS_REF@@QAA?AVsslERROR@@PBDK0PAH@Z) — parse `decl` (an
    // "EventName(args...)" event signature) and register it on the referenced class; `errHint`
    // is a source-location string logged alongside any parse/registration error; `idx` receives
    // the new event's id. Returns an sslERROR by value (sret) — register truth verified against
    // the propHALO_OBJ_DESC::InitSslClass call sites: r3 is the hidden result pointer, r4 is the
    // real `this` (this->sslClass), matching the DB's nominal (this, result, ...) ordering here
    // (unlike TypeID-style statics, this one *does* dereference `this`). Body is a boundary
    // outside this re-source. // boundary
    sslERROR RegisterEvent(const char *decl, unsigned int mask, const char *errHint, int *idx);
} sslCLASS_REF;

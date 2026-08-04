#pragma once
#include "haloENGINE_CONTROL_ssl_bridge.h"
#include "../../ws/ds/dsDATA.h"

/* ds_data_call is a generic ws-engine callback-dispatch template family (the same family that
 * backs ds_data_call::PropertyAccessorBase<T> elsewhere in this corpus) — infrastructure shared
 * across every SSL-exporter class, not something this SSL_EXPORTER_haloENGINE_CONTROL batch owns.
 * Only the one instantiation this class touches is modeled, and only down to its vtable shape
 * (DB-verified types_members "ds_data_call::HandlerBase<haloENGINE_CONTROL>" /
 * "...HandlerBase<haloENGINE_CONTROL>_vtbl"): a pure-virtual dispatcher object, exactly like this
 * corpus's existing sslCB_HOST boundary pattern. */

namespace ds_data_call {

/* Canonical template form of the DB instantiation ds_data_call::HandlerBase<haloENGINE_CONTROL>
 * (+ its _vtbl); the former flat HandlerBase_haloENGINE_CONTROL names are kept as aliases. */
template<class Host> struct HandlerBase; // forward decl for the vtbl's self-referential `this_` pointers

template<class Host>
struct HandlerBase_vtbl {
    bool (__fastcall *IsConst)(HandlerBase<Host> *this_);
    void (__fastcall *HandleCall)(HandlerBase<Host> *this_, Host *host, dsDATA *argv, int argc, dsDATA *retVal);
    void (__fastcall *dtor)(HandlerBase<Host> *this_);
};

template<class Host>
struct HandlerBase {
    HandlerBase_vtbl<Host> *__vftable; // 0x00

    // Inline forwarders through the vtable (layout-neutral: no data members). The cb* wrappers
    // spell these as `method->HandleCall(...)` / `method->IsConst()` — the decompiler's virtual-
    // call codegen — so expose them as members that dispatch through __vftable.
    bool IsConst() { return __vftable->IsConst(this); }
    void HandleCall(Host *host, dsDATA *argv, int argc, dsDATA *retVal) {
        __vftable->HandleCall(this, host, argv, argc, retVal);
    }
};

typedef HandlerBase<haloENGINE_CONTROL> HandlerBase_haloENGINE_CONTROL;
typedef HandlerBase_vtbl<haloENGINE_CONTROL> HandlerBase_haloENGINE_CONTROL_vtbl;

// CreateHandler<Host,Impl,Ret,Args...> — a compile-time factory that builds (and process-lifetime
// caches, via each caller's function-local static) a HandlerBase<haloENGINE_CONTROL> bound to a
// specific haloENGINE_CONTROL member-function pointer. 8 distinct instantiations exist in the DB
// (one per cb* wrapper's argument shape); declared here only as a boundary template — its bodies
// are ws-engine template infrastructure, out of scope for this batch. // boundary
template<class Host, class Impl, class Ret, class... Args>
HandlerBase<Host> *CreateHandler(Ret (Impl::*method)(Args...)); // boundary
// const-qualified member-function overload (GetCurLevel/GetCurCheckpoint/IsCoop are const). boundary
template<class Host, class Impl, class Ret, class... Args>
HandlerBase<Host> *CreateHandler(Ret (Impl::*method)(Args...) const); // boundary

} // namespace ds_data_call

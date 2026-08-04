#pragma once
#include "HandlerBase_haloENGINE_CONTROL.h"
#include "../../ws/ds/dsDATA.h"
#include "../../ws/ds/dsTSTRING.h"

namespace ds_data_call {

/* ArgConverter<T> — generic ws-engine ds_data_call argument unmarshaller: pulls one typed value
 * out of an argv[] dsDATA slot. Infrastructure shared across every SSL-exporter dispatch leaf, not
 * owned by this batch; only the instantiations the reversed HandleCall leaves touch are modeled,
 * and only as boundary declarations. By-value scalar forms return the value directly; class-typed
 * forms construct into a caller-provided out buffer and return a pointer to it (PPC by-value / sret
 * ABI). // boundary */
template<class T> struct ArgConverter; // primary — boundary (bodies external to this batch)

template<> struct ArgConverter<bool> { static bool Convert(dsDATA *arg); };
template<> struct ArgConverter<int>  { static int  Convert(dsDATA *arg); };
template<> struct ArgConverter<dsTSTRING<char> > {
    static dsTSTRING<char> *Convert(dsTSTRING<char> *out, dsDATA *arg);
};
template<> struct ArgConverter<const dsTSTRING<char> &> {
    static const dsTSTRING<char> *Convert(dsTSTRING<char> *out, dsDATA *arg);
};

} // namespace ds_data_call

/* ds_data_call::Handler<IsConst, Host, Ret, A1..A6> — the concrete leaf of the HandlerBase<Host>
 * callback-dispatch family (see HandlerBase_haloENGINE_CONTROL.h). A HandlerBase<Host> whose sole
 * data member is a Host member-function pointer `pf`, bound at construction; HandleCall() forwards
 * through it. DB-verified layout for every instantiation in this batch
 * (types_members "ds_data_call::Handler<...>"): __vftable @ 0x00 (inherited from HandlerBase<Host>),
 * pf @ 0x04 — 8 bytes. The trailing `void` template arguments mark unused parameter slots
 * (observed arity 0..5).
 *
 * The eight ctors (0x823C2AF0 / 2CF0 / 2D48 / 2DE8 / 2E40 / 2F40 / 2F98 / 2FF0) and the no-arg
 * HandleCall (0x823C2D88) reversed in this batch are all trivial template bodies, reproduced inline
 * below: the ctor just stores `pf` (the two base/derived vtable-pointer writes the PPC decompiler
 * shows are implicit compiler codegen for constructing HandlerBase then Handler, NOT source), and
 * HandleCall invokes the bound member function on `host`. */

namespace ds_data_call {

template<int IsConst, class Host, class Ret,
         class A1 = void, class A2 = void, class A3 = void,
         class A4 = void, class A5 = void, class A6 = void>
struct Handler : HandlerBase<Host> {
    // Bound member-function pointer @ 0x04. Its true signature is Ret(Host::*)(<the non-void A's>);
    // the trailing-void template slots make that exact type inexpressible as one uniform member
    // declaration, so it is held in the layout-identical single-inheritance member-pointer slot
    // (4 bytes on PPC/MSVC) and cast at the ctor/HandleCall boundary.
    // boundary-approx: DB layout preserved (one 4-byte member-fn-ptr); exact signature elided.
    void (Host::*pf)();

    // The eight batch ctors are all this one member-template body (each DB instantiation differs
    // only in the concrete bound member-fn-ptr signature it is called with).
    template<class BoundPF>
    Handler(BoundPF bound) : pf(reinterpret_cast<void (Host::*)()>(bound)) {}

    // 0x823C2D88 — dispatch (no-arg instantiation reversed): invoke the bound member on `host`.
    void HandleCall(Host *host, dsDATA *argv, int argc, dsDATA *retVal) {
        (void)argv; (void)argc; (void)retVal;
        (host->*pf)();
    }
};

} // namespace ds_data_call

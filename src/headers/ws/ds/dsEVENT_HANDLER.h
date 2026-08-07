#pragma once
// ws-engine ds: abstract base for an object-form dsEVENT_MGR subscriber (the ST_HANDLER arm of
// dsEVENT_MGR::ITEM_HANDLER, dispatched via OnEvent). DB-verified layout (types_members
// dsEVENT_HANDLER): __vftable@0 (dsEVENT_HANDLER_vtbl*) — size 4. Every haloENGINE_CONTROL-family
// "EVENT_DISP_*" nested dispatcher class in this corpus derives from this one leaf.
// Canonical home, and as of 2026-08-07 the SOLE definition: hcex/haloENGINE_CONTROL.h carried an
// identical plain-C mirror inside a `#else /* !__cplusplus */` branch that the include graph reaches
// from 0 C TUs (and that could not have compiled as C anyway — its own first include uses
// `extern "C"` and dsTSTRING<char>); that dead branch was deleted (odr_dup drain).
//
// DB-verified vtable (types_members dsEVENT_HANDLER_vtbl): slot0 = scalar-deleting-destructor-style
// dtor thunk, slot1 = OnEvent(const dsEVENT*). Virtual dispatch is through the explicit
// __vftable member (repo convention); declaration order == slot order.

struct dsEVENT;   // ws-engine ds: event instance passed to a subscriber (full definition in dsEVENT.h)

struct dsEVENT_HANDLER_vtbl; // boundary — ds subsystem vtable

typedef struct dsEVENT_HANDLER {
    dsEVENT_HANDLER_vtbl *__vftable; // 0x00

    // boundary — slot 0 (virtual): destructor (subclasses restore the base vtable pointer on
    // destruction, per haloENGINE_CONTROL::EVENT_DISP_haloENGINE_CONTROL::~EVENT_DISP_haloENGINE_CONTROL).
    ~dsEVENT_HANDLER();

    // boundary — slot 1 (virtual): dispatch a raised event to this subscriber. Pure at this leaf;
    // every concrete EVENT_DISP_* subclass overrides it to forward into its owning object.
    void OnEvent(const dsEVENT *event);
} dsEVENT_HANDLER;

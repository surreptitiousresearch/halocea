#pragma once
// ws-engine nav: a waypoint weighting/exclusion functor. DB-verified layout (types_members
// navWFUNC / navWFUNC_vtbl) — a single vtable pointer (size 4). The evaluators are dispatched
// through the vtable; call sites cast the slot to the concrete signature.  boundary.

struct navWP;
struct m3dV;

struct navWFUNC;

// DB-verified (types_members navWFUNC_vtbl).
struct navWFUNC_vtbl {
    void  (*dtr)(navWFUNC *self);                                              // 0x00
    bool  (*IsExcluded)(navWFUNC *self, navWP *wp, short wpid);                // 0x04
    float (*GetWeight)(navWFUNC *self, navWP *wp, const m3dV *pos, short wpid); // 0x08
};

struct navWFUNC {
    navWFUNC_vtbl *__vftable; // 0x00
};

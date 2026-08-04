#pragma once

// ws-engine ds — abstract scripted-callable handle. DB-verified layout
// (types_members dsFUNC_PTR_BASE): __vftable@0 — size 4. Bodies external.  boundary.
struct dsDATA; // ds/dsDATA.h — marshalled call args/result  boundary (fwd)

struct dsFUNC_PTR_BASE {
    void *__vftable; // 0x00 dsFUNC_PTR_BASE_vtbl*

    // vtbl — invoke the scripted callable with `argc` args, writing the result into `retVal`.
    // Returns false on failure. boundary.
    bool Call(dsDATA *args, int argc, dsDATA *retVal);
};

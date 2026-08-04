#pragma once
// ssl subsystem: ref-counted reference to a script source-text record. DB-verified layout
// (types_members sslSOURCE_REF): source@0 (sslSOURCE*) — size 4. The copy-ctor/dtor adjust the
// source refcount. Method bodies that dereference `source` live in the .cpp so this header
// only forward-declares sslSOURCE.

#include "../ds/dsTSTRING.h"

struct sslSOURCE; // ssl script source-text record (full definition in sslSOURCE.h)

typedef struct sslSOURCE_REF {
    sslSOURCE *source; // 0x00 referenced source record (null when empty)

    sslSOURCE_REF(const sslSOURCE_REF &other); // 0x82A37130 — ref-counted copy
    ~sslSOURCE_REF();                          // 0x82A370D8 — ref-counted release
    sslSOURCE_REF &operator=(const sslSOURCE_REF &other); // 0x82A37170 — ref-counted assign

    // 0x825CB9C8 — copy of the referenced source's text (returns a fresh dsTSTRING<char> by value /
    // sret); yields the shared empty string when this ref is empty. const (QBA).
    dsTSTRING<char> Get() const;

    // 0x82A37060 — retarget `source` to `p` with ref-count fixup (adds a ref to the new source,
    // releases and destroys the old one when its count hits zero). Protected (IAA).
protected:
    void _SetSourcePtr(sslSOURCE *p);
} sslSOURCE_REF;

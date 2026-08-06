#pragma once
#include "SSL_EXPORTER_haloENGINE_CONTROL.h"

/* SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry — one row of the class's static export table
 * (a (callback, id-slot, SSL declaration string) triple). DB-verified layout (types_members
 * "SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry"): cb@0 (function pointer), id@4 (int*),
 * sslDecl@8 (dsTSTRING<char>) — size 12. Defined out-of-line (nested-type split) so
 * SSL_EXPORTER_haloENGINE_CONTROL.h can forward-declare it without a circular include. */

struct SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry {
    // 0x00 the SSL-VM-facing trampoline this entry registers (self, argc, argv, retVal, caller).
    // Null for entries that register as a bare SSL event instead of a callable function.
    void (*cb)(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller);
    // 0x04 pointer to the int slot (a byte offset into an SSL_EXPORTER_haloENGINE_CONTROL instance,
    // per the ctors below) that PerformExport writes the resolved SSL function/event id into.
    int *id;
    // 0x08 the SSL declaration string, e.g. "LoadLevelSolo(map: string, difficulty: int, isNew: bool = false)".
    dsTSTRING<char> sslDecl;

    // 0x823C3814 — releases the sslDecl string buffer reference.
    ~SSLExportEntry();

    // 0x823C4530 — `scalar deleting destructor`: runs the dtor, then conditionally `operator delete`s
    // this (bit 0 of the compiler-synthesized flag argument selects delete-after-dtor).
    SSLExportEntry *ScalarDeletingDestructor(unsigned int deleteFlag);

    // 0x823C4580 — copy-construct: shares `other`'s sslDecl buffer (ref-counted), under the
    // dsTSTRING_flat processor-affinity lock.
    SSLExportEntry(const SSLExportEntry &other);

    // 0x823C4BF4 — full construct: adopts `_cb`/`_id` verbatim and shares `_sslDecl`'s buffer
    // (ref-counted).
    SSLExportEntry(
        void (*_cb)(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller),
        int *_id,
        const dsTSTRING<char> *_sslDecl);
};

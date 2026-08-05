#pragma once
// ws-engine strm (streaming): abstract base for a packed-resource container (a "pak" file that
// bundles many named sub-resources, queried by strmPACK_MANAGER::CanOpenResourceFile).
// DB-verified layout (types_members strmPACK_BASE): __vftable@0 — size 4.
// DB-verified vtable (types_members strmPACK_BASE_vtbl).

#include "../ds/dsSTRID.h"

struct fioFILE; // ../fio/fioFILE.h — boundary
struct strmPACK_BASE_vtbl;

typedef struct strmPACK_BASE {
    strmPACK_BASE_vtbl *__vftable; // 0x00

    ~strmPACK_BASE();                                                      // vtbl+0x00
    bool IsInitial();                                                      // vtbl+0x04
    bool CanOpenFile(const dsSTRID &resName, int pakId, bool *cached);     // vtbl+0x08
    fioFILE *OpenFile(const dsSTRID &resName, int pakId, int mode);        // vtbl+0x0C
    void CloseFile(fioFILE *file);                                        // vtbl+0x10
    void Term();                                                          // vtbl+0x14
} strmPACK_BASE;

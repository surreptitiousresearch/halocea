#pragma once
#include "haloENGINE_CONTROL_ssl_bridge.h"
#include "HandlerBase_haloENGINE_CONTROL.h"
#include "../../ws/ds/dsVECTOR.h"
#include "../../ws/ssl/sslOBJ_REF.h"
#include "../../ws/ssl/sslCLASS_REF.h"
#include "../../ws/ds/dsDATA.h"
#include "../../ws/ds/dsTSTRING.h"

/* SSL_EXPORTER_haloENGINE_CONTROL — the SSL script-callback export wrapper for haloENGINE_CONTROL
 * (per CLAUDE.md's "SSL script callbacks" bridge family). A singleton (Instance()) whose 13
 * `_SSL_FUNC_REGISTRATOR_*` nested one-shot helper members each push one SSLExportEntry describing
 * one exported haloENGINE_CONTROL method (a static export table shared process-wide via
 * GetExportMap()); PerformExport() then walks that table and registers each entry's SSL
 * declaration (as a callback function or a bare event, depending on whether the entry has a `cb`)
 * against a live sslCLASS_REF, stashing the resolved id back through each entry's `id` slot (an
 * offset into *this). DB-verified via `funcs`/`types_members` for every member below. */

typedef struct SSL_EXPORTER_haloENGINE_CONTROL {
    struct SSLExportEntry; // out-of-line definition: SSLExportEntry.h

    // The 13 nested one-shot static-registration helper types. Each is an empty struct whose sole
    // purpose is a constructor that PushBack()s one SSLExportEntry into GetExportMap(); named
    // after (and mangled under) the haloENGINE_CONTROL method each one exports.
    struct _SSL_FUNC_REGISTRATOR_LoadLevelSolo        { _SSL_FUNC_REGISTRATOR_LoadLevelSolo();        }; // 0x823CB918
    struct _SSL_FUNC_REGISTRATOR_LoadLevelCoop        { _SSL_FUNC_REGISTRATOR_LoadLevelCoop();        }; // 0x823CB9A0
    struct _SSL_FUNC_REGISTRATOR_LoadMainMenu         { _SSL_FUNC_REGISTRATOR_LoadMainMenu();         }; // 0x823CBA28
    struct _SSL_FUNC_REGISTRATOR_LoadCheckpoint       { _SSL_FUNC_REGISTRATOR_LoadCheckpoint();       }; // 0x823CBAB0
    struct _SSL_FUNC_REGISTRATOR_RestartCheckpoint    { _SSL_FUNC_REGISTRATOR_RestartCheckpoint();    }; // 0x823CBB38
    struct _SSL_FUNC_REGISTRATOR_RestartLevel         { _SSL_FUNC_REGISTRATOR_RestartLevel();         }; // 0x823CBBC0
    struct _SSL_FUNC_REGISTRATOR_CreateCheckpointList { _SSL_FUNC_REGISTRATOR_CreateCheckpointList(); }; // 0x823CBC48
    struct _SSL_FUNC_REGISTRATOR_SwitchInput          { _SSL_FUNC_REGISTRATOR_SwitchInput();          }; // 0x823CC980
    struct _SSL_FUNC_REGISTRATOR_SetPause             { _SSL_FUNC_REGISTRATOR_SetPause();             }; // 0x823CBD70
    struct _SSL_FUNC_REGISTRATOR_SetMode              { _SSL_FUNC_REGISTRATOR_SetMode();              }; // 0x823CBDF8
    struct _SSL_FUNC_REGISTRATOR_IsCoop               { _SSL_FUNC_REGISTRATOR_IsCoop();               }; // 0x823CBE80
    struct _SSL_FUNC_REGISTRATOR_GetCurCheckpoint     { _SSL_FUNC_REGISTRATOR_GetCurCheckpoint();     }; // 0x823CBF08
    struct _SSL_FUNC_REGISTRATOR_GetCurLevel          { _SSL_FUNC_REGISTRATOR_GetCurLevel();          }; // 0x823CBF90

    // Member instances, DB-verified layout (types_members SSL_EXPORTER_haloENGINE_CONTROL):
    // each registrator's ctor is immediately preceded by the `int` slot PerformExport resolves its
    // SSL function/event id into (offsets 0,8,16,...,96 — exactly the raw `(int*)N` byte-offset
    // constants each registrator ctor hands to SSLExportEntry::id). Declaration order here must
    // match the DB's member_index order exactly (it is also the ctor's construction order).
    // Each 1-byte registrator instance is followed by 3 bytes of DB-verified alignment padding
    // (the compiler pads the empty struct up to the next int's 4-byte boundary).
    int                                         sslFuncId_LoadLevelSolo;         // 0x00
    _SSL_FUNC_REGISTRATOR_LoadLevelSolo         _ssl_func_registrator_LoadLevelSolo;         // 0x04
    unsigned char                               _pad00[3];                                   // 0x05 db-verified padding
    int                                         sslFuncId_LoadLevelCoop;         // 0x08
    _SSL_FUNC_REGISTRATOR_LoadLevelCoop         _ssl_func_registrator_LoadLevelCoop;         // 0x0C
    unsigned char                               _pad01[3];                                   // 0x0D db-verified padding
    int                                         sslFuncId_LoadMainMenu;          // 0x10
    _SSL_FUNC_REGISTRATOR_LoadMainMenu          _ssl_func_registrator_LoadMainMenu;          // 0x14
    unsigned char                               _pad02[3];                                   // 0x15 db-verified padding
    int                                         sslFuncId_LoadCheckpoint;        // 0x18
    _SSL_FUNC_REGISTRATOR_LoadCheckpoint        _ssl_func_registrator_LoadCheckpoint;        // 0x1C
    unsigned char                               _pad03[3];                                   // 0x1D db-verified padding
    int                                         sslFuncId_RestartCheckpoint;     // 0x20
    _SSL_FUNC_REGISTRATOR_RestartCheckpoint     _ssl_func_registrator_RestartCheckpoint;     // 0x24
    unsigned char                               _pad04[3];                                   // 0x25 db-verified padding
    int                                         sslFuncId_RestartLevel;          // 0x28
    _SSL_FUNC_REGISTRATOR_RestartLevel          _ssl_func_registrator_RestartLevel;          // 0x2C
    unsigned char                               _pad05[3];                                   // 0x2D db-verified padding
    int                                         sslFuncId_CreateCheckpointList;  // 0x30
    _SSL_FUNC_REGISTRATOR_CreateCheckpointList  _ssl_func_registrator_CreateCheckpointList;  // 0x34
    unsigned char                               _pad06[3];                                   // 0x35 db-verified padding
    int                                         sslFuncId_SwitchInput;           // 0x38
    _SSL_FUNC_REGISTRATOR_SwitchInput           _ssl_func_registrator_SwitchInput;           // 0x3C
    unsigned char                               _pad07[3];                                   // 0x3D db-verified padding
    int                                         sslFuncId_SetPause;              // 0x40
    _SSL_FUNC_REGISTRATOR_SetPause              _ssl_func_registrator_SetPause;              // 0x44
    unsigned char                               _pad08[3];                                   // 0x45 db-verified padding
    int                                         sslFuncId_SetMode;               // 0x48
    _SSL_FUNC_REGISTRATOR_SetMode               _ssl_func_registrator_SetMode;               // 0x4C
    unsigned char                               _pad09[3];                                   // 0x4D db-verified padding
    int                                         sslFuncId_IsCoop;                // 0x50
    _SSL_FUNC_REGISTRATOR_IsCoop                _ssl_func_registrator_IsCoop;                // 0x54
    unsigned char                               _pad10[3];                                   // 0x55 db-verified padding
    int                                         sslFuncId_GetCurCheckpoint;      // 0x58
    _SSL_FUNC_REGISTRATOR_GetCurCheckpoint      _ssl_func_registrator_GetCurCheckpoint;      // 0x5C
    unsigned char                               _pad11[3];                                   // 0x5D db-verified padding
    int                                         sslFuncId_GetCurLevel;           // 0x60
    _SSL_FUNC_REGISTRATOR_GetCurLevel           _ssl_func_registrator_GetCurLevel;           // 0x64
    unsigned char                               _pad12[3];                                   // 0x65 db-verified padding

    // 0x823CC9F8 — private (AAA mangle) default ctor: constructs all 13 registrators in turn,
    // each pushing its SSLExportEntry into the shared static GetExportMap() table.
    SSL_EXPORTER_haloENGINE_CONTROL();

    // 0x823CCA98 — the process-wide singleton (Meyers-singleton local static, guarded).
    static SSL_EXPORTER_haloENGINE_CONTROL *Instance();

    // 0x823C6E18 — the shared static export table (Meyers-singleton local static dsVECTOR, guarded;
    // its dtor is registered via atexit — see GetExportMap_atexit_dtor.cpp).
    static dsVECTOR<SSLExportEntry, 8> *GetExportMap();

    // 0x823CB8AC — build one SSLExportEntry from (cb, id, sslDecl) and PushBack it into
    // GetExportMap(). Called both directly (RegisterEntry) and inline by each
    // _SSL_FUNC_REGISTRATOR_* ctor (which duplicates this logic rather than calling it — DB-verified,
    // not a reconstruction error).
    static void RegisterEntry(
        void (__fastcall *cb)(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller),
        int *id,
        const dsTSTRING<char> *sslDecl);

    // 0x823C9990 — walk GetExportMap(), registering each entry against `sslClass`: entries with a
    // non-null `cb` register as a callable SSL function (AddCbFunc); entries with a null `cb`
    // register as a bare SSL event (RegisterEvent). Each entry's resolved id is written back
    // through `*(int*)((char*)this + (size_t)entry->id)`. Returns 1 if every entry registered
    // without error, 0 if any failed (each failure is logged and that entry's id slot set to -1).
    // DEVIATION: the DB's raw prototype for this method shows `sslClass` as an `sslERROR*`
    // parameter and mistypes several locals as sslCLASS_REF/sslERROR interchangeably — a PPC
    // struct-return-by-value ABI artifact (sslCLASS_REF is passed by value, which this ABI
    // implements via a hidden pointer, and sslERROR sret results share the same stack slot shape).
    // Retyped to the DB-verified real parameter kind (sslCLASS_REF) in the .cpp.
    int PerformExport(sslCLASS_REF *sslClass);

    // The 13 SSL-VM-facing callback trampolines. Each resolves (once, via a function-local static
    // guard) a ds_data_call::HandlerBase_haloENGINE_CONTROL bound to the corresponding
    // haloENGINE_CONTROL member function, fetches the calling object's callback host, and dispatches
    // through it.
    static void cbRestartLevel(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller);          // 0x823C4C4C
    static void cbSetMode(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller);               // 0x823C4CEC
    static void cbIsCoop(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller);                // 0x823C4D8C
    static void cbGetCurCheckpoint(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller);      // 0x823C4E2C
    static void cbGetCurLevel(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller);           // 0x823C4ECC
    static void cbRestartCheckpoint(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller);     // 0x823C6EC4
    static void cbLoadLevelSolo(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller);         // 0x823C9B6C
    static void cbLoadMainMenu(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller);          // 0x823C9C0C
    static void cbLoadCheckpoint(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller);        // 0x823C9CAC
    static void cbLoadLevelCoop(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller);         // 0x823CB0BC
    static void cbCreateCheckpointList(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller);  // 0x823CB15C
    static void cbSetPause(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller);              // 0x823CBCDC
    static void cbSwitchInput(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller);           // 0x823CC8EC
} SSL_EXPORTER_haloENGINE_CONTROL;

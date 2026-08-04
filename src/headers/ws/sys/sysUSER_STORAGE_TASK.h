#pragma once
#include "../gts/TASK.h"
#include "../fio/fioFILE_MEM.h"
// ws-engine sys (system user / storage) subsystem: base class for an async load/save task bound
// to a sysUSER profile. BOUNDARY -- this batch only needs sysUSER_STORAGE_TASK's *size* and
// constructor/destructor signatures to type haloSTORAGE_TASK's own fields and ctor/dtor chain-up
// correctly; its own method bodies are not decompiled here.
// DB-verified layout (types_members sysUSER_STORAGE_TASK): gts::TASK (base)@0 (16 bytes),
// user@16 (ds_data::REF_TYPE<sysUSER>), type@20 (sysUSER_STORAGE_TASK::TASK_TYPE),
// contentType@24 (sysUSER_CONTENT_TYPE), stream@28 (fioFILE_MEM, 124 bytes), isAsync@152 (bool)
// -- size 156.

struct sysUSER; // ws-engine sys — boundary

// DEVIATION (2026-07 SavesTaskCreatorHCEX pass): types_enum_values now resolves these
// enumerators (previously marked unmodeled); filled in here.
enum sysUSER_CONTENT_TYPE : int { CNT_CONFIG = 0, CNT_SAVEDATA = 1, CNT_TROPHY = 2, CNT__LAST = 3 };

namespace ds_data {
// ds_data::REF_TYPE<T> is, per types_members, a single m_pData pointer (4 bytes) -- modeled
// generically here so the field below has a real (if opaque) type rather than a raw byte blob.
// Full template surface (Release/AddRef/etc) is a boundary; external to this batch.
template<class T>
struct REF_TYPE {
    void *m_pData; // 0x00 -- opaque REF_TYPE_DATA<T>*
};
}

struct sysUSER_STORAGE_TASK : gts::TASK {
    // DEVIATION (2026-07 SavesTaskCreatorHCEX pass): types_enum_values now resolves these
    // enumerators (previously marked unmodeled); filled in here.
    enum TASK_TYPE : int { SAVE = 0, LOAD = 1 };

    ds_data::REF_TYPE<sysUSER> user;        // 0x10
    TASK_TYPE            type;              // 0x14
    sysUSER_CONTENT_TYPE  contentType;       // 0x18
    fioFILE_MEM           stream;            // 0x1C (124 bytes)
    bool                  isAsync;           // 0x98

    // 0x827D8180 -- boundary: external to this batch.
    sysUSER_STORAGE_TASK(ds_data::REF_TYPE<sysUSER> u, TASK_TYPE t, sysUSER_CONTENT_TYPE ct, bool async);
    // 0x827D8248 -- boundary: external to this batch.
    virtual ~sysUSER_STORAGE_TASK();
};

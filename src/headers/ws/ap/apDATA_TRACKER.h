#pragma once
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
#include "../os/osLOCK.h"
#include "../fio/fioFILE_DISK.h"
// ws-engine ap (platform): numeric time-series data tracker. Records named per-frame value
// series and optionally streams them to a disk file. Thread-safe (holds `lock`).
// DB-verified layout (types_members apDATA_TRACKER) — size 400.

typedef struct apDATA_TRACKER {
    // DB-verified (types_members apDATA_TRACKER::RECORD): name@0 (dsTSTRING<char>),
    // values@4 (float[30]) — size 124.
    struct RECORD {
        dsTSTRING<char> name;       // 0x00
        float           values[30]; // 0x04

        // 0x82A24810 — default-construct: adopt the shared empty-string singleton for `name`.
        RECORD();

        // 0x82A24498 — copy-assign an entire record: share the source name buffer (dropping the old
        // one) and copy the 30-float value array.
        RECORD &operator=(const RECORD &other);
    };

    // fioFILE_DISK, 316 bytes — the tracker's output file (see ../fio/fioFILE_DISK.h).
    fioFILE_DISK            file;            // 0x000
    int                     frameCounter;    // 0x13C current frame slot within a RECORD
    dsVECTOR<RECORD, 8>     records;         // 0x140 named value series
    bool                    saveCaptions;    // 0x154
    bool                    sendFromConsole; // 0x155
    unsigned char           _pad0[2];        // 0x156 db-verified padding
    dsTSTRING<char>         fileName;        // 0x158
    osLOCK                  lock;            // 0x15C

    // 0x82A25558 — add `value` into the current frame slot of the series named `name`, creating
    // the series on first use. Thread-safe (holds `lock`).
    void AddValue(const char *name, float value);

    // 0x82A25470 — find the record index for `name`; if absent, create a zero-initialised record,
    // mark captions dirty, and insert it name-sorted. Private (AAA mangle).
    int FindAdd(const char *name);

    // Nonzero when the tracker's output file is open and it is recording. boundary.
    bool IsRecording() const; // boundary
} apDATA_TRACKER;

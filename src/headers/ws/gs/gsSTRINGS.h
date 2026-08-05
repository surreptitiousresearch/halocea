#pragma once
// ws-engine gs: string-table lookup system (localized/loose text keyed by a case-insensitive
// ID string, resolved to an index and a wide-char storage offset). DB-verified layout
// (types_members gsSTRINGS, ordinal 20085): leading apL10N base (vtable, 4B) then five
// dsVECTOR<T,8> members and a dsTSTRING<wchar_t> scratch buffer, total member span
// 4..108 (each dsVECTOR is 20B, dsTSTRING<wchar_t> head is 4B — trailing bytes of the
// dsTSTRING body are not expanded here, not touched by the four methods in this batch).

#include <stdint.h>
#include "apL10N.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"

struct gsSTRINGS : public apL10N {
    // gsSTRINGS::Record — a sorted string-table entry (types_members ordinal 20078).
    struct Record {
        int ofsIdText; // 0x00 index into storageIds (dsVECTOR<char,8>) where this record's ID text begins
        int idString;  // 0x04 index into mapIdxToString / string id for this record
    };

    // gsSTRINGS::PRINTF_ARG_TYPE / PRINTF_ARG — tags a single vararg captured for %-substitution
    // formatting (feeds printfArgs/printfBuf). Not touched by the four methods reconstructed in
    // this batch; layout DB-verified (types_members ordinal 20083, types_enum_values 1437).
    enum PRINTF_ARG_TYPE {
        ARG_UNKNOWN = 0,
        ARG_INT     = 1,
        ARG_DOUBLE  = 2,
        ARG_ASTR    = 3,
        ARG_WSTR    = 4,
        ARG_STRID   = 5,
    };

    struct PRINTF_ARG {
        PRINTF_ARG_TYPE type; // 0x00
        // 0x04, size 16 (widest arm: long double). Anonymous union — DB tag
        // _7E142DA013BA9B1112320DEE31FED016. Arms verified via types_members; none of them
        // are read/written by the four methods reconstructed in this batch.
        union {
            int            v_int;
            double    v_double;
            const char    *v_astr;
            const wchar_t *v_wstr;
            int            v_strid;
        };
    };

    // gsSTRINGS::RecordCmp — the compare functor FindRecord passes to
    // dsVECTOR<Record,8>::FindSorted: orders/matches a Record against a raw const char* key by
    // its stored ID text (types_members ordinal 20087).
    struct RecordCmp {
        gsSTRINGS *data; // 0x00 back-pointer to the owning table (for storageIds access)

        // 0x826B80A8 — case-insensitive compare of record.ofsIdText's text (looked up via
        // data->storageIds) against `key`. const method (QBA mangle).
        int cmp(const gsSTRINGS::Record &record, const char *key) const;
    };

    // apL10N base occupies offset 0x00 (vtable ptr, 4B) — promoted via public inheritance above.
    dsVECTOR<char, 8>                  storageIds;      // 0x04 packed, NUL-separated ID-text bytes
    dsVECTOR<wchar_t, 8>                storageStrings;  // 0x18 packed, NUL-separated wide string text
    dsVECTOR<int, 8>                    mapIdxToString;  // 0x2C string id -> offset into storageStrings
    dsVECTOR<gsSTRINGS::Record, 8>      mapTextIdToIdx;  // 0x40 sorted by ID text, searched by FindRecord
    dsVECTOR<gsSTRINGS::PRINTF_ARG, 8>  printfArgs;      // 0x54 vararg capture scratch for %-formatting
    dsTSTRING<wchar_t>                  printfBuf;       // 0x68 formatted-output scratch buffer

    // 0x826B91A0 — binary-search mapTextIdToIdx (via RecordCmp) for the record whose ID text
    // case-insensitively matches idText; returns its index or -1. Protected (I-mangle), non-const.
    int FindRecord(const char *idText);

    // 0x826B83A8 — id -> mapIdxToString[id] (an offset into storageStrings) -> wide string
    // pointer at that offset. Public, non-const (QAA mangle).
    const wchar_t *GetStringById(int id);

    // 0x826BA188 — FindRecord(idText); on a hit, returns mapTextIdToIdx[record].idString; on a
    // miss, logs a warning and returns 0. Public, non-const (QAA mangle).
    int GetStringId(const char *idText);

    // 0x826B4DD0 — reversed in gsSTRINGS__AsciiToUnicode.cpp. Copy `text` into `buf` (up to
    // `bufLen`-1 chars, NUL-terminated), widening each byte to a wchar_t.
    void AsciiToUnicode(const char *text, wchar_t *buf, int bufLen);

    // 0x826B4ED0 — reversed in gsSTRINGS__AsciiToUnicode_2.cpp. Convenience overload: widens
    // `text` into a module-local scratch buffer (unicodeBuffer[0x2000]) and returns it.
    const wchar_t *AsciiToUnicode(const char *text);
};

// ?gsStrings@@3PAVgsSTRINGS@@A — the global string-table instance.
extern gsSTRINGS *gsStrings;

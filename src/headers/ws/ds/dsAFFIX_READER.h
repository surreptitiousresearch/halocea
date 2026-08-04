#pragma once
#include "dsTSTRING.h"
// ws-engine ds: scans a string for known affixes and records located sub-strings.
// DB-verified layout (types_members dsAFFIX_READER):
//   affixStr@0, affSubStr@4 (AFF_SUBSTR[64]), nSubStr@772, curSubStr@776 — size 780.

typedef struct dsAFFIX_READER {
    // One located affix sub-string within affixStr.
    // DB-verified layout (types_members dsAFFIX_READER::AFF_SUBSTR):
    //   ptr@0, type@4, length@8 — size 12.
    struct AFF_SUBSTR {
        const char *ptr;    // 0x00
        int         type;   // 0x04
        int         length; // 0x08
    };

    dsTSTRING<char> affixStr;        // 0x000
    AFF_SUBSTR      affSubStr[64];   // 0x004
    int             nSubStr;         // 0x304
    int             curSubStr;       // 0x308

    // 0x82A75D58 — construct a reader directly over `affixes`: shares its buffer (copy-construct,
    // bumping refcount), then parses it via Init(). Callers holding a dsAFFIX_STRING pass its
    // `.str` member (or alias the whole object, since dsAFFIX_STRING is a single dsTSTRING<char>
    // at offset 0) — there is no separate ctor overload taking a dsAFFIX_STRING (DB-verified: only
    // this dsTSTRING<char> overload and the copy-ctor below exist).
    dsAFFIX_READER(const dsTSTRING<char> &affixes);

    // 0x827E5A48 — copy-construct from another reader. Body not decompiled here. boundary.
    dsAFFIX_READER(const dsAFFIX_READER &other); // boundary

    // Release the reader's copy of the affix string buffer. boundary (inlined at call sites).
    ~dsAFFIX_READER(); // boundary

    // 0x82A758D0 — parse affixStr into the affSubStr[] index (newline-split grammar).
    void Init();

    // 0x82A755A0 — test whether affix is present; caches the match into curSubStr.
    bool HaveAffix(const char *affix, int asSubString);
    // 0x82A75038 — underlying search: return the index of the recorded sub-string that matches
    // `affix` (exact-length match unless `asSubString`), or -1 if none.
    int FindAffix(const char *affix, int asSubString) const;

    // 0x82A755E0 — parse the named parameter as a float; returns false when absent.
    bool GetFloat(const char *paramName, float *val);

    // 0x82A75DB0 — copy the named parameter sub-string into `val`; returns false when absent.
    bool GetString(const char *paramName, dsTSTRING<char> *val);
    // 0x82A750F8 — for the currently-matched affix (curSubStr), scan its trailing name/value
    // sub-string pairs for a parameter named `paramName` (type-1 name entry of equal length and
    // bytes); return the index of that parameter's VALUE sub-string (name index + 1), or -1.
    int GetParam(const char *paramName) const;
} dsAFFIX_READER;

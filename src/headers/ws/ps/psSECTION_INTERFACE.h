#pragma once
#include "psSECTION.h"
#include "psSECTION_DATA.h"
#include "psARRAY.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsDATA.h"
#include "../ds/dsTSTRING.h"
struct psCOMPLEX_KEY_DATA; // parsed complex key (full layout in psCOMPLEX_KEY_DATA.h)
// ws-engine ps: the public, value-semantic accessor over a section handle. Its sole data member is
// an embedded psSECTION at offset 0 (DB: anonymous psSECTION member) — modeled as inheritance so
// the pData/handle slot is promoted.
// DB-verified layout (types_members psSECTION_INTERFACE): psSECTION @0 — size 4.

struct psSECTION_INTERFACE : psSECTION {
    // 0x82515EB0 — true when the wrapped section handle is null. const (QBA).
    int IsNull() const;

    // 0x82517708 (?IsEmpty@psSECTION_INTERFACE@@QBAHXZ) — true when this section contains no keys.
    // const (QBA).
    int IsEmpty() const;

    // 0x8251D938 (?IsKeyExist@psSECTION_INTERFACE@@QBAHPBDK@Z) — true when the (possibly dotted)
    // `key` resolves to a value in this section or its parent hierarchy. const (QBA).
    int IsKeyExist(const char *key, unsigned int flags) const;

    // 0x8251EBF0 (?GetStr@psSECTION_INTERFACE@@QBA?AV?$dsTSTRING@D@@PBDV2@K@Z) — value under `key`
    // as a dsTSTRING, falling back to `def` when absent. sret return. const (QBA).
    dsTSTRING<char> GetStr(const char *key, dsTSTRING<char> def, unsigned int flags) const;

    // 0x82516998 — number of parent sections (0 when null). const (QBA).
    int GetNParent() const;

    // 0x825169B0 — number of keys/records (0 when null). const (QBA).
    int GetNKey() const;

    // 0x82519480 — key name at record index `idx` (empty dsSTRID when null). const (QBA).
    dsSTRID GetKeyName(int idx) const;

    // 0x825194D0 — key value at record index `idx` (empty dsDATA when null). const (QBA).
    dsDATA GetKeyValue(int idx) const;

    // 0x8251ED48 — array value stored under `key` (empty psARRAY when absent). const (QBA).
    psARRAY GetArray(const char *key, unsigned int flags) const;

    // 0x8251A410 — adopt `psOwner` as this section's owner (creating an empty body if needed).
    void SetOwner(psSECTION psOwner);

    // 0x8251D870 (?GetValue@psSECTION_INTERFACE@@QBA?AVdsDATA@@PBDK@Z) — value stored under `key`
    // (parses it as a complex key, then resolves against the body). Empty dsDATA when the handle is
    // null or the key is absent. sret return. const (QBA).
    dsDATA GetValue(const char *key, unsigned int flags) const;

    // 0x8251EDB0 (?GetSection@psSECTION_INTERFACE@@QBA?AVpsSECTION@@PBDK@Z) — the section handle
    // stored under `key`, returned BY VALUE (a referenced copy); when `key` is null returns a copy
    // of this handle. Empty when absent. sret return. const (QBA). (Overload of the out-param
    // GetSection above.)
    psSECTION GetSection(const char *key, unsigned int flags) const;

    // 0x8251B390 (?IsEmptyHier@psSECTION_INTERFACE@@QBAHXZ) — true when this section and its entire
    // parent hierarchy contain no keys. const (QBA).
    int IsEmptyHier() const;

    // 0x8251D798 (?AddKey@psSECTION_INTERFACE@@QAAHPBDVdsDATA@@KH@Z) — install `val` under `key`
    // (creating an empty body first if null). Returns the leaf record index or -1. Consumes the
    // by-value `val`.
    int AddKey(const char *key, dsDATA val, unsigned int flags, int lineNmb);

    // 0x82518B70 (?SplitComplexKey@psSECTION_INTERFACE@@ABAHPBDHHAAUpsCOMPLEX_KEY_DATA@@@Z) — parse
    // the dotted key `key` into `c`'s interned sub-key chain (reversed leaf-first). Each segment is
    // interned; `internExistingOnly` makes an unknown segment fail the parse (returns 0). When
    // `lastIsSubString` is set, the trailing segment is stored verbatim as `c._psSubStr` and an
    // empty-string strid is used for the leaf key. Returns 1 on success, 0 on failure. Private const
    // (ABA); `this` is unused.
    int SplitComplexKey(const char *key, int internExistingOnly, int lastIsSubString,
                        psCOMPLEX_KEY_DATA &c) const;

    // Scratch globals recording the name / source line of the key last resolved by GetValue.
    static dsSTRID lastGetKeyName; // boundary — defined out of line
    static int     lastGetLine;    // boundary — defined out of line

    // 0x8251DC58 — value under `key` coerced to float into `*val`; returns whether it existed. const (QBA).
    int GetFloat(const char *key, float *val, unsigned int flags) const;

    // 0x8251E018 — value under `key` coerced to a section handle into `*val`; returns whether it existed. const (QBA).
    int GetSection(const char *key, psSECTION *val, unsigned int flags) const;

    // 0x8251DB98 — value under `key` coerced to int into `*val`; returns whether it existed. const (QBA).
    int GetInt(const char *key, int *val, unsigned int flags) const; // ?GetInt@psSECTION_INTERFACE@@QBAHPBDPAHK@Z

    // 0x8251DC08 — value under `key` coerced to bool into `*val`; returns whether it existed. const (QBA).
    int GetBool(const char *key, int *val, unsigned int flags) const; // ?GetBool@psSECTION_INTERFACE@@QBAHPBDPAHK@Z

    // 0x8251DF54 — value under `key` coerced to a NUL-terminated string, copied into `buf` (capacity
    // `bufLen`); returns whether it existed. const (QBA).
    int GetString(const char *key, char *buf, int bufLen, unsigned int flags) const;

    // 0x8251DEB8 — value under `key` coerced into the dsTSTRING `*val`; returns whether it existed.
    // The dsTSTRING twin of GetString (which copies into a raw char buffer). const (QBA).
    int GetDSString(const char *key, dsTSTRING<char> *val, unsigned int flags) const;

    // -- by-value-with-default convenience overloads (added by the HALO_SOUND_PARAMS re-source
    // pass; used by HALO_SOUND_PARAMS::Init to read its script-section fields). const (QBA).
    int    GetInt(const char *key, int def, unsigned int flags) const;      // ?GetInt@psSECTION_INTERFACE@@QBAHPBDHK@Z
    int    GetBool(const char *key, int def, unsigned int flags) const;     // ?GetBool@psSECTION_INTERFACE@@QBAHPBDHK@Z
    float  GetFloat(const char *key, float def, unsigned int flags) const;  // ?GetFloat@psSECTION_INTERFACE@@QBAMPBDMK@Z
    dsSTRID GetStrid(const char *key, dsSTRID def, unsigned int flags) const; // ?GetStrid@psSECTION_INTERFACE@@QBA?AVdsSTRID@@PBDV2@K@Z (sret)

    // 0x82518AF8 — compile this section back to its textual form (empty string when the handle is null). const (QBA).
    dsTSTRING<char> BuildString() const;

    // 0x82519528 — source line number of key/record `idx` (0 when null, -1 when out of range). const (QBA).
    int GetKeyLineNmb(int idx) const;

    // 0x8251AC90 — parent section handle at index `idx` (empty when null/out of range). const (QBA).
    psSECTION GetParent(int idx) const;

    // 0x8251ABF8 — owner section handle (empty when null). const (QBA).
    psSECTION GetOwner() const;

    // 0x8251D700 — add `parent` to this section's parent list (creating a body first if needed).
    void AddParent(psSECTION parent);

    // 0x8251A218 — allocate an empty section body into this handle (releasing any prior body).
    void CreateEmpty();

    // ?SetName@psSECTION_INTERFACE@@QAAXAAVdsSTRID@@ABV?$dsTSTRING@D@@@Z — intern `name` and set it
    // as this section's own (unqualified) key name, used by the legacy .ini-style parser
    // (psSECTION_PARSER_OLD::Parse) when creating a `[section]` sub-section by hand rather than via
    // AddKey. Not reversed in this batch. boundary.
    void SetName(dsSTRID &name, const dsTSTRING<char> &text);

    // ?GetFullName@psSECTION_INTERFACE@@QBA?AV?$dsTSTRING@D@@XZ — this section's dotted name
    // (walking its owner chain), used to build "key \"a.b.c\" already exist" parser diagnostics.
    // sret return: the decompiler swaps the sret slot with `this`. const (QBA). Not reversed in
    // this batch (only referenced, not requested, by the GetSection() re-source pass). boundary.
    dsTSTRING<char> GetFullName() const;

    // 0x8251F0F8 — shrink the record and parent-list tables' backing storage to exactly fit their
    // live element counts (a no-op when the handle is null).
    void Compact();

    // 0x82517008 (?CreateObject@psSECTION_INTERFACE@@QBA?AVdsDATA@@XZ) — box this section handle as
    // a dsDATA value (empty dsDATA when the handle is null). sret return: the decompiler swaps the
    // sret slot with `this`. const (QBA).
    dsDATA CreateObject() const;

    // 0x825169C8 — set/clear the record table's sort-dirty flag (a no-op when the handle is null).
    void EnableSort(int enable);

    // ?GetName@psSECTION_INTERFACE@@QBA?AVdsSTRID@@XZ — this section's own (unqualified) interned
    // name (an empty-string dsSTRID when the handle is null). sret return: the decompiler swaps the
    // sret slot with `this`. const (QBA).
    dsSTRID GetName() const;

    // 0x8251C138 (?RemoveKey@psSECTION_INTERFACE@@QAAHPBDK@Z) — remove the (possibly dotted) key
    // named by `key`. Drops this section's own body reference when doing so empties it of both
    // records and parents. Returns whether the key was found and removed.
    int RemoveKey(const char *key, unsigned int flags);

    // 0x8251B5B4 (?RemoveKey@psSECTION_INTERFACE@@QAAXH@Z) — remove record `idx` directly (no key
    // parsing). Drops this section's own body reference when doing so empties it of both records and
    // parents. A no-op when the handle is null.
    void RemoveKey(int idx);

    // 0x8251A554 — set/clear the "ignore indirectly-inherited parent lookups" flag (creating an
    // empty body first if `b` is set and the handle is null; a no-op for clearing a null handle).
    void SetIgnoreIndirectParent(int b);
};

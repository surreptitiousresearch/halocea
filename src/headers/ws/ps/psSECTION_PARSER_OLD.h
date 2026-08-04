#pragma once
#include "psSECTION.h"
#include "psSECTION_INTERFACE.h"
#include "../ds/dsTSTRING.h"
// ws-engine ps: the legacy (pre-psSECTION_PARSER) bracketed config parser — `[section]` headers,
// `key=value` lines, and `;`-prefixed comments, the same shape as classic Windows .ini files.
// DB-verified layout (types_members psSECTION_PARSER_OLD):
//   errorDesc@0 (dsTSTRING<char>), lineNmb@4 (int), lineStart@8 (const char*) — size 12.

typedef struct psSECTION_PARSER_OLD {
    dsTSTRING<char>  errorDesc; // 0x00 last parse error text
    int              lineNmb;   // 0x04 current 1-based line number
    const char      *lineStart; // 0x08 start of the buffer this line's diagnostics are relative to

    psSECTION_PARSER_OLD(); // 0x82A2D384 — default construct (empty error text)

    // 0x8308... (?AddVar@psSECTION_PARSER_OLD@@IAAHVpsSECTION@@V?$dsTSTRING@D@@1@Z) — install one
    // `key=value` pair (already stripped of surrounding whitespace) into `section` at the current
    // line. Protected (IAA). Not reversed in this batch. boundary.
    int AddVar(psSECTION section, dsTSTRING<char> key, dsTSTRING<char> value);

    // 0x8308... (?GetNextLine@psSECTION_PARSER_OLD@@IAA?AV?$dsTSTRING@D@@AAPBDPBD@Z) — read the
    // next '\n'-terminated line from `*cursor` (up to `end`), advancing `*cursor` and `lineNmb`.
    // sret return. Protected (IAA). Not reversed in this batch. boundary.
    dsTSTRING<char> GetNextLine(const char **cursor, const char *end);

    // 0x8308... (?StripSpaces@psSECTION_PARSER_OLD@@IAAXAAV?$dsTSTRING@D@@@Z) — trim leading and
    // trailing whitespace from `*s` in place. Protected (IAA). Not reversed in this batch. boundary.
    void StripSpaces(dsTSTRING<char> &s);

    // 0x83086FB0 (?Parse@psSECTION_PARSER_OLD@@QAAHPBD0VpsSECTION@@@Z) — parse the classic
    // `[section]` / `key=value` / `;comment` text in [begin,end) into `psTop`. A line whose bracketed
    // header text is exactly "#ssl" is instead treated as the start of an embedded SSL script block:
    // everything from there through `end` (or through the next line beginning with '[') becomes a
    // "#ssl" key on `psTop`. Malformed `[` / `]` brackets fail with a diagnostic in `errorDesc`;
    // otherwise this always returns 1 (individual bad `key=value` lines are silently skipped, per
    // AddVar's own — not-reversed — success/failure handling).
    int Parse(const char *begin, const char *end, psSECTION psTop);

    // 0x8251... (?GetErrorDesc@psSECTION_PARSER_OLD@@QBA?AV?$dsTSTRING@D@@XZ) — last parse error
    // text. sret return. const (QBA). Not reversed in this batch. boundary.
    dsTSTRING<char> GetErrorDesc() const;

    // 0x82A2C33... (?GetErrorLine@psSECTION_PARSER_OLD@@QBAHXZ) — line number the last error
    // occurred on. const (QBA). Not reversed in this batch. boundary.
    int GetErrorLine() const;

    // (?GetLineStart@psSECTION_PARSER_OLD@@QBAPBDXZ) — start of the buffer the current line's
    // diagnostics are relative to. const (QBA). Not reversed in this batch. boundary.
    const char *GetLineStart() const;
} psSECTION_PARSER_OLD;

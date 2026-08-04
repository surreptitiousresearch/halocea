#pragma once
#include "psSECTION.h"
#include "psSECTION_INTERFACE.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsDATA.h"
// ws-engine ps: the property-section text parser/serializer. Holds a reader state, the last error
// description, and the deferred parent-setup records built while parsing.
// DB-verified layout (types_members psSECTION_PARSER):
//   read@0 (psSECTION_PARSER::psREADER, 20 bytes), errorDesc@20 (dsTSTRING<char>),
//   parentsSetup@24 (dsVECTOR<psSECTION_PARSER::PARENT_SETUP_REC,8>, 20 bytes) — size 44.

struct psSECTION_PARSER {
    // 0x27705 psSECTION_PARSER::psREADER — the raw text cursor (DB-verified layout).
    struct psREADER {
        const char *begin;     // 0x00 start of the source buffer
        const char *pos;       // 0x04 current read cursor
        const char *end;       // 0x08 one-past-last valid byte (a synthetic NUL is read at pos==end)
        int         lineNmb;   // 0x0C current 1-based line number
        const char *lineStart; // 0x10 start of the current line (for column/diagnostics)
    };

    // 0x27709 psSECTION_PARSER::psTOKEN's anonymous value union (DB type
    // _2ABAF8E5998CDEAAF9531B8B371847CD) — the parsed literal payload for INT/FLOAT/BOOL tokens.
    union psTOKEN_VALUE {
        int                   idata;
        float                 fdata;
        unsigned char         bdata[4];
        unsigned short        wdata[2];
    };

    // 0x27710 psSECTION_PARSER::psTOKEN — one lexed token: its id (either a literal punctuation
    // character/EOF/EOL/SPACE, or one of the >255 pseudo-ids INT=257/FLOAT=258/STRING_or_BOOL=259/
    // BOOL=260 — see GetToken()/GetTokenSTRING_BOOL()), its parsed value (for INT/FLOAT/BOOL), and
    // the source-text span [begin,end) plus `unget` (the reader position the token started at, used
    // to rewind on UngetToken).
    struct psTOKEN {
        int            id;      // 0x00
        psTOKEN_VALUE  ___u1;   // 0x04 DB member name for the anonymous value union
        const char    *begin;   // 0x08 token text start
        const char    *end;     // 0x0C token text end (exclusive)
        const char    *unget;   // 0x10 reader position before this token was read

        int   &idata() { return ___u1.idata; }
        float &fdata() { return ___u1.fdata; }

        // 0x82A2D... (?asString@psTOKEN@psSECTION_PARSER@@QBA?AV?$dsTSTRING@D@@XZ) — render this
        // token's text span as a dsTSTRING (used for parser diagnostics). sret return: the
        // decompiler swaps the sret slot with `this`. Not reversed in this batch. boundary.
        dsTSTRING<char> asString() const;
    };

    // 0x27706 psSECTION_PARSER::PARENT_SETUP_REC — a deferred "this section inherits from `parent`"
    // request recorded while parsing a `parent <name>;` command; resolved once the whole file (and
    // its #include chain) has been read, so forward references to not-yet-declared parent sections
    // work.
    struct PARENT_SETUP_REC {
        psSECTION       sect;   // 0x00 the section that declared the parent relationship
        dsTSTRING<char> parent; // 0x04 the (still unresolved) parent section's dotted name

        PARENT_SETUP_REC();                              // 0x82A2D888 — default (empty parent text)
        PARENT_SETUP_REC(const PARENT_SETUP_REC &that);   // 0x82A2D114 — referenced copy
        ~PARENT_SETUP_REC();                              // 0x8251A9D4
    };

    psREADER                      read;         // 0x00 reader/cursor state
    dsTSTRING<char>               errorDesc;    // 0x14 last parse error text
    dsVECTOR<PARENT_SETUP_REC, 8> parentsSetup; // 0x18 deferred parent-setup records

    psSECTION_PARSER(); // 0x8251BFB4 — default construct (empty error text, empty parent-setup list)

    // 0x82A2D... — private helper used by GetSection() to box a multi-value `key = a, b, c;` line
    // into a psARRAY-valued dsDATA. Protected (IAA). Not reversed in this batch. boundary.
    dsDATA CreateArray(const dsVECTOR<dsDATA, 8> &values) const;

    // 0x82A2... (?GetValuesArr@psSECTION_PARSER@@IAAHAAV?$dsVECTOR@VdsDATA@@$07@@H@Z) — private
    // helper used by GetSection()/GetArrayObject() to read a comma-separated value list into `out`.
    // Protected (IAA). Not reversed in this batch. boundary.
    int GetValuesArr(dsVECTOR<dsDATA, 8> &out, int inArray);

    // 0x82A2C2B0 — pull the next raw character, advancing `read.pos`; -1 at EOF, 0 for the
    // synthetic NUL at end-of-buffer, else the unsigned byte value. Crosses lines via `read.lineNmb`
    // / `read.lineStart`.
    int GetChar();

    // 0x82A2C314 — push back `num` characters (rewinding `read.pos`, clamped at `read.begin`,
    // un-crossing lines as it goes).
    void UngetChar(int num);

    // 0x82A2C3E0 — lex a whitespace/EOF/EOL/comment run starting at the current position: consumes
    // NUL (id=EOF/0), CR/LF pairs (id=EOL/'\n'), runs of space/tab (id=SPACE/' ', rewound to just
    // before the run so the caller can decide whether to skip it), and `//`/`/* */` comments
    // (rewound to just before the comment so the caller re-lexes past it). Any other character
    // rewinds one and returns with `tok->id` left at its SPACE-loop sentinel. Returns 0 (with
    // `errorDesc` set) only via the shared EOF path in GetToken(); this helper itself always
    // succeeds.
    int GetTokenEOF_EOL_SPACE(psTOKEN &tok);

    // 0x82A2C83C — lex a numeric literal (INT or FLOAT) starting at the current character (already
    // consumed once by the caller's chain): optional leading '+'/'-', a digit run, an optional
    // '.'-led fractional digit run, an optional trailing '%' (scales the value by 0.01), and an
    // optional trailing 'f' after a fractional part. Sets tok->id to INT(257) or FLOAT(258) and
    // stores the parsed value in tok->idata/tok->fdata. Leaves the reader positioned just past the
    // literal (backs up one character first, matching the shared punctuation/word rewind). Always
    // succeeds (returns 1); a non-numeric first character leaves tok->id at its caller-supplied
    // default (256) and rewinds without consuming.
    int GetTokenINT_FLOAT(psTOKEN &tok);

    // 0x82A2CB88 — lex a quoted string ("..."), a <<<...>>> heredoc, or a bareword/BOOL token.
    // `flags` bit 2 (4) suppresses the trailing psBOOL::IsThisBoolString reclassification of a
    // bareword into a BOOL(260) token. Returns 0 (with `errorDesc` implicitly left as-is by the
    // caller) on an unterminated quoted string; 1 otherwise.
    int GetTokenSTRING_BOOL(psTOKEN &tok, unsigned int flags);

    // 0x82A2D6EC — top-level lexer: skips whitespace/EOL per `flags` (bit0 skips SPACE, bit1 skips
    // EOL), then classifies the next token as EOF, single-character punctuation (one of
    // `[]{},;=#`), a number (GetTokenINT_FLOAT), or a string/bareword/BOOL (GetTokenSTRING_BOOL).
    // Sets `tok->unget` to the pre-token reader position. Returns 0 (with `errorDesc` set to
    // "unexpected EOF" or "unknown token ...") on failure, 1 on success.
    int GetToken(psTOKEN &tok, unsigned int flags);

    // 0x82A2EF34 (?ParseCommand@psSECTION_PARSER@@IAAHVpsSECTION@@@Z) — parse one `#`-prefixed
    // parser command (`parent <name>;`, `no_parent;`, `ssl`, `ssl_begin ... #ssl_end`) into
    // `psRead` (a psSECTION handle, taken by value and used here as its psSECTION_INTERFACE view),
    // appending a deferred PARENT_SETUP_REC for `parent`. Returns 0 (with `errorDesc` set) on a
    // malformed command.
    int ParseCommand(psSECTION psRead);

    // 0x82A2FE4C — parse a `{ ... }` section body into a fresh section object, boxing it into
    // `obj`. Returns 0 (with `errorDesc` set to "closing '}' not found") if the terminating `}` is
    // missing.
    int GetSectionObject(dsDATA &obj);

    // 0x82A2E944 — parse a `[ v, v, ... ]` array body into a fresh psARRAY, boxing it into `obj`.
    // Returns 0 (with `errorDesc` set to "closing ']' not found") if the terminating `]` is missing.
    int GetArrayObject(dsDATA &obj);

    // 0x82A2F590 (?GetSection@psSECTION_PARSER@@IAAHVpsSECTION@@@Z) — parse a full section body (a
    // run of `key = value;` records, `{...}`/`[...]` sub-values, and `#`-commands) into `psRead` (a
    // psSECTION handle, taken by value and used here as its psSECTION_INTERFACE view) until a
    // closing `}` or EOF. Returns 0 (with `errorDesc` set) on any parse error (duplicate key,
    // unexpected token, missing bracket, ...).
    int GetSection(psSECTION psRead);

    // 0x82A2DAB8 (?CompileToString@psSECTION_PARSER@@SA?AV?$dsTSTRING@D@@VdsDATA@@H@Z) — serialize a
    // boxed dsDATA value tree to its textual config form at indentation level `tabOfs`. Handles
    // section ({ ... }), array ([ ... ]), string (quoted/escaped or <<<...>>> heredoc), and legacy
    // psOLD nodes; all other types are converted to a string via dsDATA::GetValue. Static; returns
    // the text by value (sret) and consumes the by-value `obj`.
    static dsTSTRING<char> CompileToString(dsDATA obj, int tabOfs);
};

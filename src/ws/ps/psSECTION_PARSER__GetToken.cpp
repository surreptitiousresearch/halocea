/* ?GetToken@psSECTION_PARSER@@IAAHAAUpsTOKEN@1@K@Z @0x82A2D6C8 */
#include "psSECTION_PARSER.h"
#include "../ds/dsTSTRING.h"
#include <string.h>

// dsTSTRING<char> free-function concatenation operators (sret / by value). boundary.
template<class T> dsTSTRING<T> operator+(const dsTSTRING<T> &lhs, const T *rhs);
template<class T> dsTSTRING<T> operator+(const dsTSTRING<T> &lhs, const dsTSTRING<T> &rhs);
template<class T> dsTSTRING<T> operator+(const T *lhs, const dsTSTRING<T> &rhs);

// 0x82A2D6EC — top-level lexer. Records `tok.unget` (the pre-token reader position, for
// UngetToken), then loops classifying SPACE/EOL/comment runs via GetTokenEOF_EOL_SPACE: comments
// are silently skipped, and SPACE/EOL are only "consumed" (looped past) when the matching `flags`
// bit says to skip them (bit0 = skip SPACE, bit1 = skip EOL) — otherwise that SPACE/EOL run itself
// becomes the returned token. Once an ordinary (non-whitespace/comment) position is reached, a
// single character is peeked: one of `[]{},;=#` or NUL becomes a one-character token outright;
// otherwise the peek is pushed back and GetTokenINT_FLOAT / GetTokenSTRING_BOOL classify what's
// there. If neither recognizes it, an "unknown token ..." diagnostic (the offending text, truncated
// to 64 characters) is recorded and the call fails.
//
// DEVIATION: the decompiler left the psTOKEN pointer passed to the GetTokenSTRING_BOOL call site as
// an uninitialized local (`v23`) — from the surrounding pattern (every other sub-lexer call in this
// function is passed `tok`) this is unambiguously meant to be `tok` as well; reconstructed as such.

int psSECTION_PARSER::GetToken(psTOKEN &tok, unsigned int flags)
{
    tok.unget = read.pos;

    for (;;) {
        if (read.pos > read.end) {
            dsTSTRING<char> msg;
            msg.pBuffer = nullptr;
            msg.UnsafeInit("unexpected EOF", -1, 0);
            errorDesc = msg;
            return 0;
        }

        tok.id = 256; // sentinel: "not yet classified"
        tok.begin = read.pos;
        tok.end = nullptr;

        if (!GetTokenEOF_EOL_SPACE(tok))
            return 0;

        int id = tok.id;
        if (tok.id == 256)
            break; // ordinary token starts here; fall through to classification below

        if ((id != ' ' || (flags & PSGT_SKIP_SPACES) == 0) && (id != '\n' || (flags & PSGT_SKIP_EOLS) == 0))
            goto finalize; // report this SPACE/EOL token as-is

        // else: SPACE/EOL is being skipped per `flags` — loop and lex the next run
    }

    {
        int c = GetChar();
        if (strchr("[]{},;=#", c) || !c) {
            tok.id = c;
            goto finalize;
        }
        UngetChar(1); // put back the classification peek

        if (!GetTokenINT_FLOAT(tok))
            return 0;
        if (tok.id != 256)
            goto finalize;

        if (!GetTokenSTRING_BOOL(tok, flags))
            return 0;
        if (tok.id != 256)
            goto finalize;

        // Truly unrecognized: build an "unknown token \"...\"" diagnostic from the first bytes.
        const char *begin = tok.begin;
        const char *end = read.end;
        if (end >= begin + 64)
            end = begin + 64;
        tok.end = end;

        dsTSTRING<char> tokenText;
        tokenText.pBuffer = nullptr;
        tokenText.UnsafeInit(begin, (int)(end - begin), 0);
        errorDesc = operator+<char>(operator+<char>("unknown token \"", tokenText), "\"...");
        // ("unknown token \"" + tokenText) + "\"..."
        return 0;
    }

finalize:
    if (!tok.end)
        tok.end = read.pos;
    return 1;
}

/* ?GetTokenEOF_EOL_SPACE@psSECTION_PARSER@@IAAHAAUpsTOKEN@1@@Z @0x82A2C3D0 */
#include "psSECTION_PARSER.h"
// 0x82A2C3E0 — lex a whitespace/EOF/EOL/comment run at the current reader position. Classifies the
// next raw character(s):
//   NUL              -> tok.id = 0 (EOF); done.
//   CR then LF, or LF then CR -> the pair collapses to one EOL; a lone CR or LF also counts.
//                       tok.id = '\n' (10); done.
//   run of ' '/'\t'  -> tok.id = ' ' (32); the one non-whitespace lookahead character that ended
//                       the run is pushed back (UngetChar(1)) so it is re-lexed by the caller.
//   "//" ... EOL/EOF -> line comment, consumed silently through (and including) the terminator,
//                       which is then pushed back for the caller to re-lex; tok.id left untouched.
//   "/*" ... "*/"    -> block comment, consumed silently through the closing "*/" (or EOF);
//                       tok.id left untouched.
//   '/' <anything else> -> not a comment: both peeked characters are pushed back; tok.id untouched.
//   anything else    -> a single character is pushed back; tok.id untouched (the caller classifies
//                       whatever ordinary token starts here).
// Always succeeds (returns 1) — the EOF-with-no-more-input diagnostic lives in the caller, GetToken.

int psSECTION_PARSER::GetTokenEOF_EOL_SPACE(psTOKEN &tok)
{
    int c = GetChar();

    if (c == 0) {
        tok.id = 0;
        return 1;
    }

    if (c == '\r' || c == '\n') {
        int pairChar = (c == '\r') ? '\n' : '\r';
        if (GetChar() != pairChar)
            UngetChar(1);
        tok.id = '\n';
        return 1;
    }

    while (c == ' ' || c == '\t') {
        tok.id = ' ';
        c = GetChar();
    }
    if (tok.id == ' ') {
        UngetChar(1); // put back the non-whitespace lookahead character
        return 1;
    }

    if (c != '/') {
        UngetChar(1);
        return 1;
    }

    int c2 = GetChar();
    if (c2 == '/') {
        // Line comment: consume through the next CR/LF/EOF, then push the terminator back.
        while (c2 != '\r' && c2 > 0 && c2 != '\n')
            c2 = GetChar();
        UngetChar(1);
        return 1;
    }
    if (c2 != '*') {
        // Not a comment after all: put back both peeked characters.
        UngetChar(2);
        return 1;
    }

    // Block comment: consume through the matching "*/" (or true EOF).
    for (;;) {
        int cc = GetChar();
        if (cc < 0)
            break;
        if (cc == '*' || cc == 0) {
            int cc2 = GetChar();
            if (cc2 < 0 || cc2 == '/')
                break;
        }
    }
    return 1;
}

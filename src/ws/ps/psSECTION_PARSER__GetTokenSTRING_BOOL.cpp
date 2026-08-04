#include "psSECTION_PARSER.h"
#include "psBOOL.h"
// 0x82A2CB88 — lex a quoted string ("..."), a "<<<...>>>" heredoc, or a bareword (which may then be
// reclassified as a BOOL literal). `flags` bit 2 (4) suppresses that trailing BOOL reclassification.
//
// DEVIATION: the compiler fused the quoted-string scan and the bareword scan into one shared,
// backslash-escape-aware character loop with intertwined gotos. This reproduces the same observable
// behavior (which characters are consumed, when scanning stops, when it fails) with two separate,
// readable loops instead of the single fused one.

int psSECTION_PARSER::GetTokenSTRING_BOOL(psTOKEN &tok, unsigned int flags)
{
    int c = GetChar();
    bool quoted = false;

    if (c == '"') {
        quoted = true;
        c = GetChar();
    } else if (c == '<') {
        if (GetChar() == '<') {
            if (GetChar() == '<') {
                // Confirmed "<<<" heredoc opener: scan for the closing ">>>".
                int scan = c;
                for (;;) {
                    while (scan > 0 && scan != '>')
                        scan = GetChar();
                    int c2 = GetChar();
                    if (c2 <= 0)
                        return 0; // unterminated heredoc
                    if (c2 == '>') {
                        int c3 = GetChar();
                        if (c3 <= 0)
                            return 0;
                        if (c3 == '>') {
                            tok.begin += 3;
                            tok.end = read.pos - 3;
                            tok.id = 259; // STRING
                            return 1;
                        }
                        scan = c3;
                    } else {
                        scan = c2;
                    }
                }
            }
            UngetChar(2); // only "<<" matched: undo both lookahead reads
        } else {
            UngetChar(1); // only "<" matched: undo the one lookahead read
        }
        // The leading '<' itself stays consumed either way; fall through to the word/quote scan.
    }

    if (!quoted) {
        // Bareword: consume a run of word/identifier-ish characters (honoring a one-character
        // backslash escape before each), then push back whatever ended the run.
        for (;;) {
            if (c == '\\')
                c = GetChar();
            bool wordy = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                         (c >= '0' && c <= '9') || c == '_' || c == '.' || c == '"' ||
                         c == '|' || c == '%' || c == '*' || c == '#';
            if (!wordy)
                break;
            c = GetChar();
        }
        UngetChar(1);
    } else {
        // Quoted string: consume through an unescaped, undoubled closing '"'.
        for (;;) {
            if (c == '\\') {
                c = GetChar(); // escaped character: consumed verbatim, no further checks on it
                c = GetChar();
                continue;
            }
            if ((unsigned int)c < 0x20)
                return 0; // control character / EOF: unterminated string
            if (c == '"') {
                if (GetChar() != '"')
                    break; // a lone, undoubled quote ends the string
                c = GetChar(); // "" is an escaped literal quote: keep scanning
                continue;
            }
            c = GetChar();
        }
    }

    tok.id = 259; // STRING (subject to BOOL reclassification below)

    if (quoted) {
        ++tok.begin;             // skip the opening quote
        tok.end = read.pos - 1;  // exclude the closing quote
        return 1;
    }

    if ((flags & 4) == 0) {
        unsigned int boolData = 0;
        if (psBOOL::IsThisBoolString(tok.begin, (int)(read.pos - tok.begin), &boolData)) {
            tok.id = 260; // BOOL
            tok.idata() = (int)boolData;
        }
    }
    return 1;
}

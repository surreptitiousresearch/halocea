#include "psSECTION_PARSER.h"
// 0x82A2C83C — lex a numeric literal (INT or FLOAT). Grammar: an optional leading '+'/'-', a run of
// decimal digits, an optional '.'-led fractional digit run (upgrades the token to FLOAT and may be
// followed by a trailing 'f' suffix), and an optional trailing '%' (which also forces FLOAT and
// scales the accumulated value by 0.01). The character that terminates the literal is pushed back
// (except when it's the trailing '%', which is consumed). If the very first character isn't a sign
// followed by a digit, or a digit outright, no literal is recognized: the reader is rewound all the
// way back to the entry position (`saved`) and `tok.id` is left however the caller set it, so the
// caller can try a different classification. Always returns 1.

int psSECTION_PARSER::GetTokenINT_FLOAT(psTOKEN &tok)
{
    psREADER saved = read; // backtrack point for the "not a number" case

    int c = GetChar();
    bool negative = (c == '-');
    if (c == '-' || c == '+')
        c = GetChar();

    int intVal = 0;
    if (c >= '0') {
        while (c <= '9') {
            tok.id = 257; // INT
            intVal = intVal * 10 + (c - '0');
            c = GetChar();
            if (c < '0')
                break;
        }
    }

    float floatVal = (float)intVal;

    if (c == '.') {
        if (tok.id == 257)
            tok.id = 258; // upgrade INT -> FLOAT
        c = GetChar();
        float scale = 0.1f;
        if (c >= '0') {
            while (c <= '9') {
                tok.id = 258; // FLOAT
                floatVal += (float)(c - '0') * scale;
                scale *= 0.1f;
                c = GetChar();
                if (c < '0')
                    break;
            }
        }
        if (c == 'f')
            c = GetChar();
    }

    if (c == '%') {
        tok.id = 258; // FLOAT
        floatVal *= 0.01f;
        tok.fdata() = negative ? -floatVal : floatVal;
        return 1;
    }

    UngetChar(1); // put back the character that terminated the literal

    if (tok.id == 257) {
        tok.idata() = negative ? -intVal : intVal;
        return 1;
    }
    if (tok.id == 258) {
        tok.fdata() = negative ? -floatVal : floatVal;
        return 1;
    }

    // No digits at all: not a numeric literal — fully back out.
    read = saved;
    return 1;
}

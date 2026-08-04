#include "psSECTION_PARSER.h"
#include "psSECTION.h"
#include "psSECTION_INTERFACE.h"
#include "../ds/dsDATA.h"
#include "../ds/dsTSTRING.h"
#include <string.h>

// dsTSTRING<char> free-function concatenation operators (sret / by value). boundary.
template<class T> dsTSTRING<T> operator+(const dsTSTRING<T> &lhs, const T *rhs);
template<class T> dsTSTRING<T> operator+(const dsTSTRING<T> &lhs, const dsTSTRING<T> &rhs);
template<class T> dsTSTRING<T> operator+(const T *lhs, const dsTSTRING<T> &rhs);

// 0x82A2EF34 — parse one `#`-prefixed command. `psRead` is the enclosing section (taken by value,
// released automatically at scope exit); it's the section a `parent`/`no_parent`/`#ssl*` command
// applies to.
//
// Recognized commands:
//   parent <name>;   -- defer "psRead inherits from <name>" (resolved later, see PARENT_SETUP_REC).
//   no_parent;       -- mark psRead to ignore any indirectly-inherited parent lookups.
//   ssl              -- the rest of the current line (through EOF) becomes psRead's "#ssl" key.
//   ssl_begin        -- everything up to the next "#ssl_end" marker (trailing whitespace/newline
//                        trimmed) becomes psRead's "#ssl" key.
// Anything else is an "unknown command" error.

int psSECTION_PARSER::ParseCommand(psSECTION psRead)
{
    psSECTION_INTERFACE &sect = static_cast<psSECTION_INTERFACE &>(psRead);

    psTOKEN cmdTok;
    if (!GetToken(cmdTok, 4))
        return 0;
    if (cmdTok.id != 259) {
        errorDesc = operator+<char>(
            operator+<char>("unexpected token \"", cmdTok.asString()), "\", waiting cmd name");
        return 0;
    }

    dsTSTRING<char> cmd;
    cmd.pBuffer = nullptr;
    cmd.UnsafeInit(cmdTok.begin, (int)(cmdTok.end - cmdTok.begin), 0);
    const char *cmdStr = cmd.pBuffer->str;

    if (strcmp(cmdStr, "parent") == 0) {
        psTOKEN nameTok;
        if (!GetToken(nameTok, 5))
            return 0;
        if (nameTok.id != 259) {
            errorDesc = operator+<char>(
                operator+<char>("unexpected token \"", nameTok.asString()),
                "\", waiting parent section");
            return 0;
        }
        PARENT_SETUP_REC rec;
        rec.sect = psRead;
        rec.parent = nameTok.asString();
        parentsSetup.PushBack(rec);
        return 1;
    }

    if (strcmp(cmdStr, "no_parent") == 0) {
        sect.SetIgnoreIndirectParent(1);
        return 1;
    }

    if (strcmp(cmdStr, "ssl") == 0) {
        dsTSTRING<char> sslText;
        sslText.pBuffer = nullptr;
        sslText.UnsafeInit(read.pos, (int)(read.end - read.pos), 0);
        read.pos = read.end;
        int lineNmb = read.lineNmb;

        dsDATA boxed;
        boxed.type = nullptr;
        boxed.StoreValue<dsTSTRING<char>>(sslText);
        sect.AddKey("#ssl", boxed, 0, lineNmb);
        sect.Compact();
        return 1;
    }

    if (strcmp(cmdStr, "ssl_begin") == 0) {
        const char *marker = strstr(read.pos, "#ssl_end");
        while (marker) {
            const char *lineBegin = read.pos;
            const char *afterMarker = marker + 8; // strlen("#ssl_end")
            const char *p = marker;
            while (--p > lineBegin) {
                int ch = (unsigned char)*p;
                if (ch != ' ' && ch != '\t') {
                    if (p > lineBegin && (*p == '\n' || *p == '\r')) {
                        dsTSTRING<char> sslText;
                        sslText.pBuffer = nullptr;
                        sslText.UnsafeInit(lineBegin, (int)(p - lineBegin), 0);
                        read.pos = afterMarker;
                        int lineNmb = read.lineNmb;

                        dsDATA boxed;
                        boxed.type = nullptr;
                        boxed.StoreValue<dsTSTRING<char>>(sslText);
                        sect.AddKey("#ssl", boxed, 0, lineNmb);
                        sect.Compact();
                        return 1;
                    }
                    break;
                }
            }
            marker = strstr(afterMarker, "#ssl_end");
        }
        dsTSTRING<char> msg;
        msg.pBuffer = nullptr;
        msg.UnsafeInit("no #ssl_end found", -1, 0);
        errorDesc = msg;
        return 0;
    }

    errorDesc = operator+<char>(operator+<char>("unknown command \"", cmd), "\"");
    return 0;
}

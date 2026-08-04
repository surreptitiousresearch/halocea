#include "psSECTION_PARSER_OLD.h"
#include "psSECTION.h"
#include "psSECTION_INTERFACE.h"
#include "../ds/dsDATA.h"
#include "../ds/dsSTRID.h"
#include <string.h>

// dsTSTRING<char> free-function concatenation operator (sret / by value). boundary.
template<class T> dsTSTRING<T> operator+(const dsTSTRING<T> &lhs, const dsTSTRING<T> &rhs);
template<class T> dsTSTRING<T> operator+(const dsTSTRING<T> &lhs, const T *rhs);

// 0x83086FB0 — parse classic `[section]` / `key=value` / `;comment` text into `psTop`.
//
// DEVIATION: the "No left/right bracket" diagnostics literally append the *raw offending line text*
// after the fixed message (not its line number, despite the message wording) — reproduced here
// exactly as observed rather than "corrected" to a line number, since that's what the binary does.

int psSECTION_PARSER_OLD::Parse(const char *begin, const char *end, psSECTION psTop)
{
    psSECTION_INTERFACE &top = static_cast<psSECTION_INTERFACE &>(psTop);

    lineStart = begin;
    lineNmb = 0;

    dsTSTRING<char> empty;
    empty.pBuffer = nullptr;
    empty.UnsafeInitEmpty();
    errorDesc = empty;

    if (begin == end)
        return 1;

    const char *cursor = begin;

    for (;;) {
        dsTSTRING<char> line = GetNextLine(&cursor, end);

        if (line.pBuffer->strLen == 0)
            continue; // blank line

        if (line.pBuffer->str[0] == ';')
            continue; // comment line

        if (line.pBuffer->str[0] != '[') {
            errorDesc = operator+<char>("No left bracket at line ", line);
            return 0;
        }

        int closeIdx = line.Find("]", 1, 0);
        if (closeIdx < 0) {
            errorDesc = operator+<char>("No right bracket at line ", line);
            return 0;
        }

        dsTSTRING<char> sectionName = line.SubStr(1, closeIdx - 1);
        StripSpaces(sectionName);

        if (strcmp(sectionName.pBuffer->str, "#ssl") == 0)
            break; // embedded SSL script: handled after the loop

        // Ordinary [section] header: create (or find an existing) sub-section and name/own it.
        psSECTION sub;
        static_cast<psSECTION_INTERFACE &>(sub).CreateEmpty();
        dsDATA boxed = static_cast<psSECTION_INTERFACE &>(sub).CreateObject();
        if (!top.AddKey(sectionName.pBuffer->str, boxed, 0, lineNmb))
            sub = top.GetSection(sectionName.pBuffer->str, 0);

        static_cast<psSECTION_INTERFACE &>(sub).SetOwner(psTop);
        dsSTRID nameId;
        static_cast<psSECTION_INTERFACE &>(sub).SetName(nameId, sectionName);

        // Read key=value lines until the next [section] header or EOF.
        while (cursor != end) {
            const char *lineStartMark = cursor;
            dsTSTRING<char> kv = GetNextLine(&cursor, end);

            if (kv.pBuffer->strLen == 0 || kv.pBuffer->str[0] == ';')
                continue;

            if (kv.pBuffer->str[0] == '[') {
                cursor = lineStartMark; // push this line back for the outer loop to re-read
                break;
            }

            int eq = kv.Find("=", 0, 0);
            dsTSTRING<char> key, value;
            key.pBuffer = nullptr;
            value.pBuffer = nullptr;
            if (eq < 0) {
                key.UnsafeInitEmpty();
            } else {
                key = kv.SubStr(0, eq);
            }
            StripSpaces(key);
            if (eq < 0) {
                value.UnsafeInitEmpty();
            } else {
                value = kv.SubStr(eq + 1, -1);
            }
            StripSpaces(value);

            if (!AddVar(sub, key, value))
                return 0;
        }

        if (cursor == end)
            return 1;
    }

    // "[#ssl]" header: everything from here through `end` becomes psTop's "#ssl" key.
    dsTSTRING<char> sslText;
    sslText.pBuffer = nullptr;
    sslText.UnsafeInit(cursor, (int)(end - cursor), 0);

    dsDATA boxed;
    boxed.type = nullptr;
    boxed.StoreValue<dsTSTRING<char>>(sslText);
    top.AddKey("#ssl", boxed, 0, lineNmb);
    return 1;
}

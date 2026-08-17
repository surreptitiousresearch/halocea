/* ?GetSection@psSECTION_PARSER@@IAAHVpsSECTION@@@Z @0x82A2F560 */
#include "psSECTION_PARSER.h"
#include "psSECTION.h"
#include "psSECTION_INTERFACE.h"
#include "psARRAY.h"
#include "../ds/dsDATA.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"

// 0x8200155A — interned empty-string literal used to build an empty-key AddKey call for a bare
// `{ ... }` (no key name) entry. boundary.
extern const char psEmptyStringLiteral[];

// Return `s` (a raw token text span) with backslash escapes resolved (sret / by value). boundary.
extern dsTSTRING<char> psClearEscSym(const dsTSTRING<char> &s);

// dsTSTRING<char> free-function concatenation operators (sret / by value). boundary.
template<class T> dsTSTRING<T> operator+(const dsTSTRING<T> &lhs, const T *rhs);
template<class T> dsTSTRING<T> operator+(const dsTSTRING<T> &lhs, const dsTSTRING<T> &rhs);
template<class T> dsTSTRING<T> operator+(const T *lhs, const dsTSTRING<T> &rhs);

// 0x82A2F590 — parse a full section body. `psRead` is taken by value (its own referenced handle,
// released automatically at scope exit) and used throughout as its psSECTION_INTERFACE view.
//
// Grammar per record, read as a token stream:
//   ';'                       -> empty statement, skip.
//   '#' <command>             -> ParseCommand().
//   '{' <section> '}'         -> anonymous (empty-key) nested section value.
//   <name> [ <name> ...] '{' <section> '}'   -> `name[.name...] = { nested section }`.
//   <name> [ <name> ...] '=' <v> [, <v>]* ';'? -> `name[.name...] = value` (a run of comma values
//                                                 boxes as a psARRAY when there's more than one).
// Any other token (typically the closing '}') ends the section: it is pushed back for the caller to
// consume, and the record table is compacted/sorted before returning success.

int psSECTION_PARSER::GetSection(psSECTION psRead)
{
    psSECTION_INTERFACE &sect = static_cast<psSECTION_INTERFACE &>(psRead);

    dsTSTRING<char> keyText;
    keyText.pBuffer = nullptr;
    keyText.UnsafeInitEmpty();
    keyText.Reserve(32);

    dsVECTOR<dsDATA, 8> values;
    values.Reserve(8);

    psTOKEN tok;
    if (!GetToken(tok, 7))
        return 0;

    for (;;) {
        // --- skip ';' / handle '#'-commands / handle anonymous "{ ... }" entries ---
        for (;;) {
            if (tok.id == ';')
                break; // empty statement: fall through to "read next record"

            int lineNmb = read.lineNmb;

            if (tok.id == '#') {
                if (!ParseCommand(psRead))
                    return 0;
                break;
            }

            if (tok.id != '{')
                goto have_key_start; // real token: exit both loops, start key-name collection

            {
                dsDATA sub;
                sub.type = nullptr;
                if (!GetSectionObject(sub)) {
                    if (sub.type)
                        sub.type->Destroy(&sub.storage);
                    return 0;
                }
                dsDATA boxed;
                boxed.type = nullptr;
                boxed.StoreValue(sub);
                sect.AddKey(psEmptyStringLiteral, boxed, 0, lineNmb);
                sect.EnableSort(0);
                if (sub.type)
                    sub.type->Destroy(&sub.storage);
            }
            break;
        }

        if (!GetToken(tok, 7))
            return 0;
        continue;

    have_key_start:
        if (tok.id != 259) {
            // Not a key-name token (typically the closing '}'): push it back for the caller.
            UngetChar((int)(read.pos - tok.unget));
            sect.Compact();
            sect.EnableSort(1);
            return 1;
        }

        {
            dsTSTRING<char> piece;
            piece.pBuffer = nullptr;
            piece.UnsafeInit(tok.begin, (int)(tok.end - tok.begin), 0);
            keyText = psClearEscSym(piece);
        }

        int lineNmb = read.lineNmb;

        // Collect a possibly dotted key name: consecutive bareword/string tokens with no operator
        // between them continue the same key ("a" "b" "c" -> "a.b.c").
        for (;;) {
            if (!GetToken(tok, 5))
                return 0;
            if (tok.id != 259)
                break;
            dsTSTRING<char> piece;
            piece.pBuffer = nullptr;
            piece.UnsafeInit(tok.begin, (int)(tok.end - tok.begin), 0);
            dsTSTRING<char> unescaped = psClearEscSym(piece);
            keyText = operator+<char>(operator+<char>(keyText, "."), unescaped);
        }

        if (tok.id == '{') {
            dsDATA sub;
            sub.type = nullptr;
            if (!GetSectionObject(sub))
                return 0;
            dsDATA boxed;
            boxed.type = nullptr;
            boxed.StoreValue(sub);
            if (sect.AddKey(keyText.pBuffer->str, boxed, 0, lineNmb)) {
                sect.EnableSort(0);
                if (sub.type)
                    sub.type->Destroy(&sub.storage);
                if (!GetToken(tok, 7))
                    return 0;
                continue;
            }
            // Duplicate key.
            dsTSTRING<char> fullName = sect.GetFullName();
            errorDesc = operator+<char>(
                operator+<char>(operator+<char>(operator+<char>("key \"", fullName), "."), keyText),
                "\" already exist");
            if (sub.type)
                sub.type->Destroy(&sub.storage);
            return 0;
        }

        if (tok.id == '=') {
            values.Clear();
            if (!GetValuesArr(values, 0))
                return 0;

            if (values.nElem > 1) {
                dsDATA arrBoxed = CreateArray(values);
                values[0].StoreValue(arrBoxed);
                if (arrBoxed.type)
                    arrBoxed.type->Destroy(&arrBoxed.storage);
            }

            if (values.nElem) {
                dsDATA valBoxed;
                valBoxed.type = nullptr;
                valBoxed.StoreValue(values[0]);
                if (sect.AddKey(keyText.pBuffer->str, valBoxed, 0, lineNmb)) {
                    sect.EnableSort(0);
                    if (!GetToken(tok, 7))
                        return 0;
                    continue;
                }
                dsTSTRING<char> fullName = sect.GetFullName();
                errorDesc = operator+<char>(
                    operator+<char>(operator+<char>(operator+<char>("key \"", fullName), "."), keyText),
                    "\" already exist");
                return 0;
            }

            dsTSTRING<char> msg;
            msg.pBuffer = nullptr;
            msg.UnsafeInit("no values found", -1, 0);
            errorDesc = msg;
            return 0;
        }

        // Unexpected token.
        errorDesc = operator+<char>(operator+<char>("unexpected token \"", tok.asString()), "\"");
        return 0;
    }
}

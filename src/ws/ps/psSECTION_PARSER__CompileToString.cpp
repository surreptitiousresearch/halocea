/* ?CompileToString@psSECTION_PARSER@@SA?AV?$dsTSTRING@D@@VdsDATA@@H@Z @0x82A2DAB8 */
#include "psSECTION_PARSER.h"
#include "psSECTION.h"
#include "psSECTION_INTERFACE.h"
#include "psARRAY.h"
#include "psOLD.h"
#include "../ds/dsDATA.h"
#include "../ds/dsDATA_TYPE.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsTSTRING_BUF_HEADER.h"
#include "../ds/dsCONVERTION_TYPE.h"
#include "../ds/ds_boundary.h"

// 0x82A2DAB8 — serialize a boxed dsDATA value tree to its textual config form at indentation level
// `tabOfs`. Sections render as `{ ... }` with one "key    =    value" line per record (the special
// "#ssl" key is wrapped in #ssl_begin/#ssl_end); arrays render as `[ ... ]` with comma-separated,
// recursively compiled elements; strings render either quoted+escaped or, when they contain an early
// newline, as a `<<< ... >>>` heredoc; legacy psOLD nodes are quoted; any other type is converted to
// a string via dsDATA::GetValue. Static; the by-value `obj` is destroyed before return (sret result).

// 0x8200155A — interned empty-string literal used to build an empty dsSTRID. boundary.
extern const char psEmptyStringLiteral[];
// Debug allocator free — not needed explicitly here (temporaries destruct themselves). boundary.
extern "C" void dlFree(void *ptr);

// ps text helpers (not reversed in this batch). boundary.
// Render a dsDATA value into `out` as raw text (used for the #ssl embedded-script block).
extern void psDataToString(const dsDATA &value, dsTSTRING<char> *out);
// Return `s` with config special characters escaped (sret / by value).
extern dsTSTRING<char> psAddEscSym(const dsTSTRING<char> &s);

// dsTSTRING<char> free-function concatenation operators (sret / by value). boundary.
template<class T> dsTSTRING<T> operator+(const dsTSTRING<T> &lhs, const T *rhs);
template<class T> dsTSTRING<T> operator+(const dsTSTRING<T> &lhs, const dsTSTRING<T> &rhs);
template<class T> dsTSTRING<T> operator+(const dsTSTRING<T> &lhs, T rhs);
template<class T> dsTSTRING<T> operator+(T lhs, const dsTSTRING<T> &rhs);
template<class T> dsTSTRING<T> operator+(const T *lhs, const dsTSTRING<T> &rhs);

dsTSTRING<char> psSECTION_PARSER::CompileToString(dsDATA obj, int tabOfs)
{
    dsTSTRING<char> result;
    result.pBuffer = nullptr;
    result.UnsafeInitEmpty();
    result.Reserve(256);

    const dsDATA_TYPE *type = obj.type;

    if (type == dsDATA_TYPE_STORAGE<psSECTION>::dataType) {
        // --- Section value: "{ key = value ... }" ---
        psSECTION section(obj);
        psSECTION_INTERFACE &sect = (psSECTION_INTERFACE &)section;
        if (!sect.IsNull()) {
            dsTSTRING<char> indent;
            indent.pBuffer = nullptr;
            indent.UnsafeInitEmpty();
            indent.Insert(0, '\t', tabOfs); // `tabOfs` tab characters of indentation

            if (tabOfs > 0)
                result.Insert(result.pBuffer->strLen, "{\n", -1);

            for (int i = 0; i < sect.GetNKey(); ++i) {
                dsSTRID keyName = sect.GetKeyName(i);
                if (keyName == "#ssl") {
                    // Embedded SSL script: emit it verbatim inside #ssl_begin/#ssl_end markers.
                    dsTSTRING<char> sslText;
                    sslText.pBuffer = nullptr;
                    sslText.UnsafeInitEmpty();
                    dsDATA sslValue = sect.GetKeyValue(i);
                    psDataToString(sslValue, &sslText);
                    if (sslValue.type)
                        sslValue.type->Destroy(&sslValue.storage);

                    dsTSTRING<char> block =
                        indent + "#ssl_begin\n" + indent + sslText + "\n" + indent + "#ssl_end\n";
                    result.Insert(result.pBuffer->strLen, block.pBuffer->str, block.pBuffer->strLen);
                } else {
                    // Ordinary key: recursively compile the value, then emit "key    =    value".
                    dsTSTRING<char> childText =
                        psSECTION_PARSER::CompileToString(sect.GetKeyValue(i), tabOfs + 1);
                    if (childText.pBuffer->strLen == 0)
                        result.Insert(result.pBuffer->strLen, "//err: ", -1);

                    dsSTRID emptyKey(psEmptyStringLiteral, 1);
                    if (emptyKey.id == keyName.id)
                        result += indent + childText + '\n'; // anonymous key: value only
                    else
                        result += indent + keyName.CStr() + "    =    " + childText + '\n';
                }
            }

            if (tabOfs > 0) {
                // Close the block with the indent reduced by one level.
                indent.Erase(tabOfs - 1, 1);
                dsTSTRING<char> closing = indent + "}";
                result.Insert(result.pBuffer->strLen, closing.pBuffer->str, closing.pBuffer->strLen);
            }
        }
        // psSECTION `section` releases its reference on scope exit.
    } else if (type == dsDATA_TYPE_STORAGE<psARRAY>::dataType) {
        // --- Array value: "[ elem, elem, ... ]" ---
        psARRAY array(obj);
        dsTSTRING<char> indent;
        indent.pBuffer = nullptr;
        indent.UnsafeInitEmpty();
        indent.Insert(0, '\t', tabOfs);

        result.Insert(result.pBuffer->strLen, "[\n", -1);
        for (int j = 0; j < array.Length(); ++j) {
            if (j > 0)
                result.Insert(result.pBuffer->strLen, ",\n", -1);
            dsTSTRING<char> childText =
                psSECTION_PARSER::CompileToString(array.GetElement(j), tabOfs + 1);
            dsTSTRING<char> line = indent + childText;
            result.Insert(result.pBuffer->strLen, line.pBuffer->str, line.pBuffer->strLen);
        }

        // Closing bracket on its own line with the indent reduced by one level (clamped at 0).
        indent.Clear();
        indent.Insert(0, '\t', (tabOfs - 1 < 0) ? 0 : tabOfs - 1);
        dsTSTRING<char> closing = "\n" + indent + "]";
        result.Insert(result.pBuffer->strLen, closing.pBuffer->str, closing.pBuffer->strLen);
        // psARRAY `array` releases its reference on scope exit.
    } else if (type == dsDATA_TYPE_STORAGE<dsTSTRING<char>>::dataType) {
        // --- String value ---
        dsTSTRING<char> str;
        str.pBuffer = nullptr;
        str.UnsafeInitEmpty();
        obj.GetValue<dsTSTRING<char>>(str, DSD_CONV_RETRIEVE);

        int len = str.pBuffer->strLen;
        int newlinePos = -1;
        for (int k = 0; k < len; ++k) {
            if (str.pBuffer->str[k] == '\n') {
                newlinePos = k;
                break;
            }
        }

        if (newlinePos >= 0 && newlinePos < 5) {
            // Early newline: emit as a <<< ... >>> heredoc block.
            result = "<<<" + str + ">>>";
        } else {
            // No newline, or first newline at column >= 5: emit quoted and escaped.
            dsTSTRING<char> escaped = psAddEscSym(str);
            result = '"' + escaped + '"';
        }
    } else if (type == dsDATA_TYPE_STORAGE<psOLD>::dataType) {
        // --- Legacy psOLD node: quote the current text if it contains a quote character. ---
        // (result is freshly empty here, so this branch is effectively a no-op in practice.)
        if (result.pBuffer->strLen > 0) {
            int len = result.pBuffer->strLen;
            int quotePos = -1;
            for (int k = 0; k < len; ++k) {
                if (result.pBuffer->str[k] == '"') {
                    quotePos = k;
                    break;
                }
            }
            if (quotePos != -1) {
                dsTSTRING<char> escaped = psAddEscSym(result);
                result = '"' + escaped + '"';
            }
        }
    } else {
        // --- Any other type: convert the value directly to a string. ---
        obj.GetValue<dsTSTRING<char>>(result, DSD_CONV_EXPLICIT);
    }

    // Destroy the by-value `obj` copy this function owns.
    if (obj.type)
        obj.type->Destroy(&obj.storage);
    return result;
}

/* ?DoProcess@psPREPROCESSOR@@IAAHXZ @0x82A2FF78 */
#include "psPREPROCESSOR.h"
#include "../ds/dsCMP.h"
#include <string.h>

// dsTSTRING<char> free-function concatenation operator (sret / by value). boundary.
template<class T> dsTSTRING<T> operator+(const T *lhs, const dsTSTRING<T> &rhs);
// 0x82A2FF88 — run the `#ifdef`/`#ifndef`/`#else`/`#elifdef`/`#elifndef`/`#endif` preprocessor over
// [begin,end), accumulating surviving text into `result`. Text is copied verbatim except while
// `skipCount` is nonzero (inside a currently-false conditional block). A line whose first word is
// `#` followed by one of the six recognized directives is consumed as a directive; any OTHER `#...`
// line (including the section parser's own `#ssl`/`#ssl_begin`/`parent`/etc. commands) is passed
// through into `result` verbatim, unconsumed by this preprocessor.
//
// Per-`#if...` state, one byte pushed on `stack`:
//   1 = this block's condition matched: currently emitting, still eligible for a later #else/#elif
//       to take over (by then skipping this one).
//   2 = an `#else` was taken after an earlier matched branch: currently emitting, no more elif/else
//       transitions expected.
//   3 = this block's condition did NOT match: currently skipping, still eligible for a later
//       #else/#elif to start emitting.
//   4 = an earlier branch already matched and was taken: this #else/#elif is permanently skipped.
//
// DEVIATION: the decompiled CRLF-pairing check right after the "skip to end of line" scan compares
// the byte *before* the found terminator against the terminator itself, which does not describe a
// CRLF/LFCR pair skip in any way that survives re-reading; reconstructed here as the conventional
// (and almost certainly intended) behavior instead: after stopping at a CR or LF terminator, consume
// it, and also consume one further paired LF/CR immediately following it.

int psPREPROCESSOR::DoProcess()
{
    result.Reserve((int)(end - begin));

    const char *p = begin;
    if (p >= end)
        return 1;

    for (;;) {
        // Copy verbatim text up to the next '#' or NUL, honoring skipCount.
        for (; p < end; ++p) {
            char c = *p;
            if (!c || c == '#')
                break;
            if (!skipCount)
                result.Insert(result.pBuffer->strLen, c, 1);
        }
        if (p == end || !*p)
            return 1;

        // Read the directive word (e.g. "#ifdef") and, after skipping spaces/tabs, the macro-name
        // word that may follow it on the same line.
        dsTSTRING<char> directive;
        directive.pBuffer = nullptr;
        directive.UnsafeInitEmpty();
        for (; p < end; ++p) {
            char c = *p;
            if (!c || c == '\n' || c == '\r' || c == ' ' || c == '\t')
                break;
            directive.Insert(directive.pBuffer->strLen, c, 1);
        }
        for (; p < end; ++p) {
            char c = *p;
            if (!c || (c != ' ' && c != '\t'))
                break;
        }
        dsTSTRING<char> macroName;
        macroName.pBuffer = nullptr;
        macroName.UnsafeInitEmpty();
        for (; p < end; ++p) {
            char c = *p;
            if (!c || c == '\n' || c == '\r' || c == ' ' || c == '\t')
                break;
            macroName.Insert(macroName.pBuffer->strLen, c, 1);
        }
        // Consume the rest of the line (ignored) up to its terminator, then the terminator itself
        // (and a paired CR/LF, if any).
        for (; p < end; ++p) {
            char c = *p;
            if (!c || c == '\n' || c == '\r')
                break;
        }
        if (p < end && (*p == '\n' || *p == '\r')) {
            char first = *p;
            ++p;
            if (p < end && *p != first && (*p == '\n' || *p == '\r'))
                ++p;
        }

        bool isDefined = macro.Find<dsCMP, dsTSTRING<char>>(macroName, dsCMP(), 0) != -1;
        const char *cmdStr = directive.pBuffer->str;

        if (strcmp(cmdStr, "#ifdef") == 0) {
            if (isDefined) {
                stack.PushBack(1);
            } else {
                stack.PushBack(3);
                ++skipCount;
            }
        } else if (strcmp(cmdStr, "#ifndef") == 0) {
            if (!isDefined) {
                stack.PushBack(1);
            } else {
                stack.PushBack(3);
                ++skipCount;
            }
        } else if (strcmp(cmdStr, "#endif") == 0) {
            unsigned char top = stack.Back();
            if (!top) {
                dsTSTRING<char> msg;
                msg.pBuffer = nullptr;
                msg.UnsafeInit("Unexpected #endif", -1, 0);
                errorDesc = msg;
                return 0;
            }
            if (top == 3 || top == 4)
                --skipCount;
            if (stack.nElem)
                stack.ShrinkResize(stack.nElem - 1);
        } else if (strcmp(cmdStr, "#else") == 0) {
            unsigned char top = stack.Back();
            if (top == 1) {
                stack.Back() = 4;
                ++skipCount;
            } else if (top == 3) {
                --skipCount;
                stack.Back() = 2;
            } else {
                dsTSTRING<char> msg;
                msg.pBuffer = nullptr;
                msg.UnsafeInit("Unexpected #else", -1, 0);
                errorDesc = msg;
                return 0;
            }
        } else if (strcmp(cmdStr, "#elifdef") == 0 || strcmp(cmdStr, "#elifndef") == 0) {
            bool isIfdefForm = (strcmp(cmdStr, "#elifdef") == 0);
            unsigned char top = stack.Back();
            if (top == 1) {
                stack.Back() = 4;
                ++skipCount;
            } else if (top == 3) {
                --skipCount;
                bool thisMatches = isIfdefForm ? isDefined : !isDefined;
                if (thisMatches) {
                    stack.Back() = 1;
                } else {
                    stack.Back() = 3;
                    ++skipCount;
                }
            } else {
                dsTSTRING<char> msg;
                msg.pBuffer = nullptr;
                msg.UnsafeInit("Unexpected #else", -1, 0);
                errorDesc = msg;
                return 0;
            }
        } else {
            // Not one of ours (e.g. the section parser's own "#ssl"/"parent"/etc.): pass through.
            result += directive;
            if (macroName.pBuffer->strLen) {
                dsTSTRING<char> withSpace = " " + macroName;
                result += withSpace;
            }
        }

        if (p >= end)
            return 1;
    }
}

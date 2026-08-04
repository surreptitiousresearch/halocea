#pragma once
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
// ws-engine ps: the `#ifdef`/`#ifndef`/`#else`/`#elifdef`/`#elifndef`/`#endif` text preprocessor run
// over a ".ps" config file's raw text before the section parser ever sees it. `macro` is the active
// set of defined macro names (set by the caller before DoProcess() runs); `stack` tracks the nested
// if/else state (one byte per open `#if...`: 0 = taking the "then" branch and it matched, 1 = taking
// the "then" branch (didn't match, still scanning for an else/elif), 2 = already-taken branch closed
// (an `#else` seen after a matching branch), 3 = currently skipping this branch (condition false),
// 4 = permanently skipping (an outer branch never matched)).
// DB-verified layout (types_members psPREPROCESSOR):
//   stack@0 (dsVECTOR<unsigned char,8>, 20 bytes), skipCount@20 (int), result@24 (dsTSTRING<char>),
//   errorDesc@28 (dsTSTRING<char>), macro@32 (dsVECTOR<dsTSTRING<char>,8>, 20 bytes), begin@52
//   (const char*), end@56 (const char*) — size 60.

typedef struct psPREPROCESSOR {
    dsVECTOR<unsigned char, 8>   stack;     // 0x00 nested #if/#else state, one byte per open block
    int                          skipCount; // 0x14 nesting depth currently being skipped (0 = emitting)
    dsTSTRING<char>              result;    // 0x18 accumulated preprocessed output text
    dsTSTRING<char>              errorDesc; // 0x1C last error text
    dsVECTOR<dsTSTRING<char>, 8> macro;     // 0x20 active `#define`d macro names
    const char                  *begin;     // 0x34 start of the input text
    const char                  *end;       // 0x38 end of the input text

    psPREPROCESSOR();  // 0x82A2D420 — default construct (empty stack/result/errorDesc/macro list)
    ~psPREPROCESSOR(); // 0x82A2D218

    // 0x82A2FF88 — run the preprocessor over [begin,end), accumulating the surviving text into
    // `result`. Returns 0 (with `errorDesc` set) on an unbalanced/misplaced `#else`/`#elifdef`/
    // `#elifndef`/`#endif`, or an unterminated `#if...` block reaching EOF unremarked (the latter is
    // not itself an error in the observed binary — see the .cpp for the exact directive semantics).
    int DoProcess();
} psPREPROCESSOR;

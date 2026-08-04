#pragma once
#include "psSECTION.h"
#include "psSECTION_KEY_REF.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
// ws-engine ps: a one-shot ".ps" script file load result — the parsed root section, an error
// message (if parsing failed), any accumulated `#macro` definitions, and the preprocessor
// state used for `#include` handling. Constructed on the stack, filled in by
// psSYSTEM::_LoadPsFromFile, and torn down at scope exit.
// DB-verified layout (types_members psSYSTEM): multipleAccessGroups@0 (unsigned int, used here
// as a bool "load succeeded" result), errorDesc@4 (dsTSTRING<char>), macro@8
// (dsVECTOR<dsTSTRING<char>,8>), includePreprocessor@28 (INCLUDE_PREPROCESSOR*) — size 32.

// ps-parser #include/file loader. Only its vtable is used here: slot at vtbl+4 reads a named file
// into a freshly-allocated dsTSTRING buffer, returning nonzero on success. The rest of the object
// (its parse/#include state) is external to this batch. boundary.
struct INCLUDE_PREPROCESSOR {
    struct VTBL {
        void *reserved0;                                        // vtbl+0
        int (*LoadFile)(INCLUDE_PREPROCESSOR *self,             // vtbl+4
                        const dsTSTRING<char> &fileName,
                        dsTSTRING<char> *outContents);
    } *vtbl;                                                    // 0x00
};

typedef struct psSYSTEM {
    unsigned int                     multipleAccessGroups; // 0x00 nonzero on successful parse
    dsTSTRING<char>                  errorDesc;              // 0x04
    dsVECTOR<dsTSTRING<char>, 8>     macro;                    // 0x08
    INCLUDE_PREPROCESSOR             *includePreprocessor;       // 0x1C

    // Disasm-verified args (0x836C3550 call site; decompiler mis-typed two of the trailing
    // dsTSTRING<char> slots as raw addresses due to a stack-slot reuse artifact — DEVIATION:
    // reconstructed here as `this, fileName, name, errHint` per the mangled callee signature
    // `_LoadPsFromFile_psSYSTEM__QAA_AVpsSECTION__V__dsTSTRING_D__00_Z`, i.e.
    // (psSECTION, dsTSTRING<char> fileName, dsTSTRING<char> name, dsTSTRING<char> errHint) with
    // `this`=result in r3). Parses `fileName` into a fresh root section; `errHint` receives a
    // human-readable error on failure. Body belongs to the Blam ".ps" tag-script parser
    // subsystem, a sibling of the HALO_SOUND_SYSTEM bridge — external to this batch. boundary.
    void _LoadPsFromFile(psSECTION rootSection, const dsTSTRING<char> &fileName,
                          const dsTSTRING<char> &name, const dsTSTRING<char> &errHint);

    // 0x825B4780 (?LoadPsFromFile@psSYSTEM@@QAA?AVpsSECTION@@V?$dsTSTRING@D@@00H@Z) — read
    // `fileName` through the #include-preprocessor, then parse it via LoadPsFromString. Appends a
    // per-record note to errorDesc on parse failure. Returns the parsed root section by value.
    // `ignorePreload` is present in the ABI but unused by the body.
    psSECTION LoadPsFromFile(dsTSTRING<char> fileName, dsTSTRING<char> name,
                             dsTSTRING<char> errHint, int ignorePreload);

    // ?LoadPsFromString@psSYSTEM@@QAA?AVpsSECTION@@PBDV?$dsTSTRING@D@@11@Z — parse the in-memory
    // text `str` into a root section, using the by-value name/errHint/fileName strings for
    // diagnostics. Returns the parsed section by value. Body external to this batch. boundary.
    psSECTION LoadPsFromString(const char *str, dsTSTRING<char> name,
                               dsTSTRING<char> errHint, dsTSTRING<char> fileName);
} psSYSTEM;

// The default #include-preprocessor instance (psINCLUDE_PREPROCESSOR psIPP). boundary.
extern INCLUDE_PREPROCESSOR psIPP;

// The global root section every top-level ".ps" file is parsed relative to. boundary.
// NOTE: the underlying symbol (?psSystem@@3PAVpsSYSTEM@@A) is actually a psSYSTEM* (see
// aiPLANNER::ProcessINIT / hcex_effect_names_parse, which load its pointer value and pass it as
// the LoadPsFromFile `this`). This declaration is kept as psSECTION for the txm consumer's
// existing boundary modeling; consumers needing the psSYSTEM* view declare it locally.
extern psSECTION psSystem;


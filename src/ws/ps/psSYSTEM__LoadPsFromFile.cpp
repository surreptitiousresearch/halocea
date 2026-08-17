/* ?LoadPsFromFile@psSYSTEM@@QAA?AVpsSECTION@@V?$dsTSTRING@D@@00H@Z @0x825B4780 */
#include "psSYSTEM.h"
#include "psSECTION.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsTSTRING_BUF_HEADER.h"
// 0x825B4780 (?LoadPsFromFile@psSYSTEM@@QAA?AVpsSECTION@@V?$dsTSTRING@D@@00H@Z) —
// read the file `fileName` through the #include-preprocessor into a text buffer, then parse it via
// LoadPsFromString. On parse failure the returned section is empty and a "\nwhile loading
// ps-pak-record <name>" note is appended to this->errorDesc. Returns the parsed root section by
// value (sret). `ignorePreload` is carried in the ABI slot but unused by this body.
//
// DEVIATION: the decompiler swaps the sret return with `this` (spelling the psSYSTEM `this` as
// `result`) and mistypes several dsTSTRING stack copies as raw addresses — a stack-slot-reuse
// artifact. Reconstructed here per the disassembly (sret psSECTION in r3, psSYSTEM* this in r4,
// then by-value dsTSTRING fileName/name/errHint) and the mangled callee signatures.

extern "C" void dlFree(void *ptr);                       // boundary — dlmalloc allocator free
extern "C"      void *memcpy(void *dst, const void *src, unsigned int n);

// The default #include-preprocessor instance, adopted when the caller left one unset. boundary.
extern INCLUDE_PREPROCESSOR psIPP;

// ?dsSPrintf@@YA?AV?$dsTSTRING@D@@PBDZZ — formatted-string builder (sret dsTSTRING). boundary.
extern dsTSTRING<char> dsSPrintf(const char *fmt, ...);

psSECTION psSYSTEM::LoadPsFromFile(dsTSTRING<char> fileName, dsTSTRING<char> name,
                                   dsTSTRING<char> errHint, int ignorePreload)
{
    psSECTION parsedRoot;                     // sret return value
    dsTSTRING<char> fileText;                 // preprocessor output (the file's expanded text)

    if (!this->includePreprocessor)
        this->includePreprocessor = &psIPP;

    if (this->includePreprocessor->vtbl->LoadFile(this->includePreprocessor, fileName, &fileText)) {
        // File read succeeded — parse its text. The three dsTSTRING args are passed by value.
        psSECTION parsed = this->LoadPsFromString(fileText.CStr(), name, errHint, fileName);

        if (!parsed.pData) {
            // Parse failed: annotate the accumulated error description with the record name.
            dsTSTRING<char> note = dsSPrintf("\nwhile loading ps-pak-record %s", name.CStr());
            int pos = this->errorDesc.pBuffer->strLen;
            this->errorDesc.Insert(pos, note.pBuffer->strLen);
            memcpy(this->errorDesc.pBuffer->str + pos, note.pBuffer->str, note.pBuffer->strLen);
            if (--note.pBuffer->refCount == 0)
                dlFree(note.pBuffer);
        }

        parsedRoot = parsed;                  // referenced copy into the sret slot
    } else {
        parsedRoot = psSECTION();             // empty section on read failure
    }

    return parsedRoot;
}

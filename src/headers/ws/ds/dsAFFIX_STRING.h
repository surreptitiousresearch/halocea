#pragma once
#include "dsTSTRING.h"
// ws-engine ds: affix (decorated) string. DB-verified layout
// (types_members dsAFFIX_STRING): a single unnamed dsTSTRING<char> at offset 0 — size 4.

typedef struct dsAFFIX_STRING {
    dsTSTRING<char> str; // 0x00 (unnamed in DB)

    // 0x823B1458 — default-construct as an empty affix string: `str` default-member-inits to the
    // shared empty-string singleton, then the body re-adopts a second empty temporary and assigns
    // it over `str` via operator= (redundant-looking but matches the compiled sequence exactly).
    dsAFFIX_STRING();
    // 0x8252D2E0 — construct from an existing string: `str` default-member-inits to empty, then
    // the body assigns `initial` over it via operator=.
    dsAFFIX_STRING(const dsTSTRING<char> &initial);
    // Destructor: releases `str`'s buffer reference. Body not decompiled here. // boundary
    ~dsAFFIX_STRING(); // boundary

    // 0x823B10D0 — copy-assign: forwards to dsTSTRING<char>::operator= on `str`.
    dsAFFIX_STRING &operator=(const dsAFFIX_STRING &that);
} dsAFFIX_STRING;

#pragma once
#include "dsTSTRING.h"
#include "dsVECTOR.h"
// ws-engine ds: accumulates named affixes (each with a parameter list) for emission.
// DB-verified layout (types_members dsAFFIX_BUILDER): affixes@0 (dsVECTOR<AFFIX,8>) — size 20.

typedef struct dsAFFIX_BUILDER {
    // DB-verified (types_members dsAFFIX_BUILDER::AFF_PARAM): name@0, value@4 — size 8.
    // Direct nested type of dsAFFIX_BUILDER per the DB path (NOT nested in AFFIX).
    struct AFF_PARAM {
        dsTSTRING<char> name;  // 0x00
        dsTSTRING<char> value; // 0x04

        // 0x82A760D0 — default ctor (empty name/value buffers).
        AFF_PARAM();
    };

    // A single affix: a name plus a parameter vector.
    // DB-verified (types_members dsAFFIX_BUILDER::AFFIX): name@0, params@4 — size 24.
    struct AFFIX {
        dsTSTRING<char>           name;   // 0x00
        dsVECTOR<AFF_PARAM, 8>    params; // 0x04

        // Default-construct a blank affix (empty name buffer, empty params). // boundary
        AFFIX();

        // 0x82A778D0 — copy-construct: share that name buffer (bump refcount), deep-copy params.
        AFFIX(const AFFIX &that);

        // 0x82A77968 — copy-assign: drop this name buffer, share that one, deep-copy params.
        AFFIX &operator=(const AFFIX &that);
    };

    dsVECTOR<AFFIX, 8> affixes; // 0x00

    // 0x82A78D50 (called from mtlMTL_MNG::FilterAffixStringOverride at 0x82A86EE8) — parsing
    // constructor: split `affixString` into component affixes with parameters. Uses the same
    // name/paramName/paramValue line-cycle grammar as dsAFFIX_READER::Init; each completed name
    // or {paramName,paramValue} pair is committed via Add().
    dsAFFIX_BUILDER(const dsTSTRING<char> &affixString);

    // ??0dsAFFIX_BUILDER@@QAA@XZ — default ctor (empty builder).
    dsAFFIX_BUILDER();

    // 0x82A78758 — copy one affix (by id) out of another builder into this one.
    void AddAffixFrom(const dsAFFIX_BUILDER &ab, int idAffix);

    // Return the id of the affix named `name`, or -1 if none exists. // boundary
    int FindAffix(const dsTSTRING<char> &name) const;

    // 0x82A786C0 — return the id of the affix named `name`, appending a fresh empty affix
    // with that name if it is not already present.
    int Add(const dsTSTRING<char> &name);

    // 0x82A77760 — set the parameter named `name` to `value` on affix `affixIndex`: no-op if
    // either string is empty; if a parameter with that name already exists, overwrite its value;
    // otherwise append a fresh {name, value} parameter.
    void Add(int affixIndex, const dsTSTRING<char> &name, const dsTSTRING<char> &value);

    // 0x82A79510 — emit the built affixes as a single string (const; returns dsTSTRING<char> by
    // value / sret). Copies the affix vector, sorts it, then appends each affix in sorted order.
    dsTSTRING<char> BuildString() const;

    // 0x82A76F98 — append one affix to the output string being built: emit its name and a
    // newline, then each parameter as "name\n value\n", then a trailing newline. Static; the
    // mangle takes both operands by reference.
    static void AppendAffix(dsTSTRING<char> &out, const AFFIX &affix);
} dsAFFIX_BUILDER;

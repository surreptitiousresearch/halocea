/* ?BuildString@psSECTION_INTERFACE@@QBA?AV?$dsTSTRING@D@@XZ @0x82518AF8 */
#include "psSECTION_INTERFACE.h"
#include "psSECTION.h"
#include "../ds/dsDATA.h"
#include "../ds/dsTSTRING.h"
// 0x82518AF8 — compile this section back to its textual form. When the handle is null the result is
// the shared empty string; otherwise the section is boxed into a dsDATA and handed to the parser's
// serializer. sret return: the decompiler swaps the sret slot with `this`.

// ?CompileToString@psSECTION_PARSER@@SA?AV?$dsTSTRING@D@@VdsDATA@@H@Z — serialize a boxed section
// value to text (static, returns dsTSTRING<char> by value). boundary.
extern dsTSTRING<char> psSECTION_PARSER_CompileToString(const dsDATA &value, int flags);

dsTSTRING<char> psSECTION_INTERFACE::BuildString() const
{
    if (this->pData) {
        dsDATA sectionValue;
        sectionValue.type = nullptr; // raw: only the type slot is zeroed before StoreValue installs it
        sectionValue.StoreValue<psSECTION>(*this); // slice psSECTION_INTERFACE -> psSECTION base
        return psSECTION_PARSER_CompileToString(sectionValue, 0);
    }
    dsTSTRING<char> result;
    // DB literal empty_string is the shared empty-string byte; UnsafeInit(-1,0) yields an empty string.
    result.pBuffer = nullptr;
    result.UnsafeInit("", -1, 0);
    return result;
}

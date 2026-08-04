#include <string.h>
#include "dsAFFIX_BUILDER.h"
#include "dsTSTRING.h"
#include "dsTSTRING_BUF_HEADER.h"

// dsAFFIX_BUILDER::AppendAffix @ 0x82A76F98
// Append one affix to the output string: emit its name and a newline, then every parameter as
// "name\n value\n", then a final newline. Each field is appended by opening a gap in `out` with the
// protected grow-and-shift Insert(pos, len) and memcpy'ing the characters into it directly.
// The mangle takes both operands by reference.
void dsAFFIX_BUILDER::AppendAffix(dsTSTRING<char> &out, const AFFIX &affix)
{
    // affix name
    int at = out.pBuffer->strLen;
    out.Insert(at, affix.name.pBuffer->strLen); // protected grow-gap Insert
    memcpy(&out.pBuffer->str[at], affix.name.pBuffer->str, affix.name.pBuffer->strLen);
    out.Insert(out.pBuffer->strLen, "\n", -1);

    // each parameter: name then value, each followed by a newline
    for (int i = 0; i < affix.params.nElem; ++i) {
        const AFF_PARAM &param = affix.params[i];

        int nameAt = out.pBuffer->strLen;
        out.Insert(nameAt, param.name.pBuffer->strLen);
        memcpy(&out.pBuffer->str[nameAt], param.name.pBuffer->str, param.name.pBuffer->strLen);
        out.Insert(out.pBuffer->strLen, "\n", -1);

        int valueAt = out.pBuffer->strLen;
        out.Insert(valueAt, param.value.pBuffer->strLen);
        memcpy(&out.pBuffer->str[valueAt], param.value.pBuffer->str, param.value.pBuffer->strLen);
        out.Insert(out.pBuffer->strLen, "\n", -1);
    }

    // blank line terminating the affix block
    out.Insert(out.pBuffer->strLen, "\n", -1);
}

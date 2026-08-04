#include "dsAFFIX_BUILDER.h"
#include <string.h> // memcpy

// dsAFFIX_BUILDER::dsAFFIX_BUILDER(const dsTSTRING<char>&) @ 0x82A78D50
// Parsing constructor: split affixString into affixes-with-parameters, using the same line-cycle
// grammar as the sibling dsAFFIX_READER::Init (name line, then alternating paramName/paramValue
// lines; a blank line resets to a fresh affix-name expectation). A completed name (state 0) or a
// completed {paramName, paramValue} pair (state 2) is committed via Add(); Add(name) is idempotent
// (returns the existing id when the name repeats) and Add(id, name, value) no-ops on an empty
// operand, so the state-0 call is a harmless no-op the first time through a group (paramName/
// paramValue are still the cleared/empty scratch strings from the previous group's reset).
//
// CAVEAT: after Lock()+memcpy, the decompile re-derives the just-copied line's strLen by rescanning
// for an embedded NUL (or falling back to maxStrLen) instead of trusting Lock()'s own contract
// (dsTSTRING<char>::Lock is documented to already truncate-and-NUL-terminate at the requested
// length). This reads as the decompiler duplicating Lock()'s own tail inline rather than genuine
// caller-side logic, so it is treated here as a single Lock(lineEnd - pos) call per its documented
// contract; flagging in case the duplicated block was in fact deliberate caller-side logic.
dsAFFIX_BUILDER::dsAFFIX_BUILDER(const dsTSTRING<char> &affixString)
{
    this->affixes.pData = nullptr;
    this->affixes.nElem = 0;
    this->affixes.allocated = 0;
    this->affixes.__cl.file = "D:\\Projects\\code\\common\\src.sys\\ds\\ds_affixes.cpp";
    this->affixes.__cl.line = 755;

    dsTSTRING<char> lineBuf[3]; // [0]=affix name, [1]=param name, [2]=param value
    for (int i = 0; i < 3; ++i)
        lineBuf[i].UnsafeInitEmpty();

    int lineType = 0; // 0=name, 1=paramName, 2=paramValue -- same cycle as dsAFFIX_READER::Init
    int affixId = 0;
    int pos = 0;
    while (pos >= 0) {
        int affixTextLen = affixString.pBuffer->strLen;
        if (pos >= affixTextLen)
            break;

        int lineEnd = pos;
        while (affixString.pBuffer->str[lineEnd] != '\n') {
            if (++lineEnd >= affixTextLen)
                goto done; // no terminating newline: drop the partial trailing line
        }
        if (lineEnd < 0)
            break;

        if (lineEnd - pos <= 0) {
            // blank line: end the current group and reset to expect a fresh affix name
            lineType = 0;
            lineBuf[0].Clear();
            lineBuf[2].Clear();
            lineBuf[1].Clear();
        } else {
            dsTSTRING<char> &target = lineBuf[lineType];
            char *dst = target.Lock(lineEnd - pos);
            memcpy(dst, &affixString.pBuffer->str[pos], lineEnd - pos);

            if (lineType == 0 || lineType == 2) {
                affixId = this->Add(lineBuf[0]);
                this->Add(affixId, lineBuf[1], lineBuf[2]);
                lineType = 1;
            } else {
                lineType = 2;
            }
        }
        pos = lineEnd + 1;
    }
done:
    ; // lineBuf[2..0] release their buffers via ~dsTSTRING() in reverse declaration order
}

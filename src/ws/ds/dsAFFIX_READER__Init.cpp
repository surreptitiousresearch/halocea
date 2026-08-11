#include "dsAFFIX_READER.h"
#include "ds_assert_boundary.h"

// dsAFFIX_READER::Init @ 0x82A758D0
// Parse affixStr into the affSubStr[] index by splitting on newlines. The sub-string type cycles per
// affix group: the first line of a group is the affix name (type 0), then alternating parameter-name
// (type 1) / parameter-value (type 2) lines. An empty line ends the group and resets the type to 0.
// A trailing partial line (no newline) is ignored. Parsing stops at 64 recorded sub-strings.
void dsAFFIX_READER::Init()
{
    int lineType = 0; // 0 = affix name, 1 = parameter name, 2 = parameter value
    int pos = 0;
    while (pos >= 0) {
        dsTSTRING_BUF_HEADER<char> *buffer = this->affixStr.pBuffer;
        if (pos >= this->affixStr.pBuffer->strLen)
            break;

        int lineEnd = pos;
        while (buffer->str[lineEnd] != '\n') {
            if (++lineEnd >= this->affixStr.pBuffer->strLen)
                return; // no terminating newline: drop the partial trailing line
        }
        if (lineEnd < 0)
            break;

        if (lineEnd - pos <= 0) {
            lineType = 0; // empty line ends the current affix group
        } else {
            this->affSubStr[this->nSubStr].ptr = &buffer->str[pos];
            this->affSubStr[this->nSubStr].type = lineType;
            this->affSubStr[this->nSubStr].length = lineEnd - pos;

            // advance the cycle: name(0)->paramName(1)->paramValue(2)->paramName(1)->...
            if (lineType == 0 || lineType == 2)
                lineType = 1;
            else
                lineType = 2;

            int newCount = this->nSubStr + 1;
            this->nSubStr = newCount;
            if (!IGNORE_STRONG_ASSERT && newCount > 64)
                static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                    "nSubStr <= subStrCount",
                    "D:\\Projects\\code\\common\\src.sys\\ds\\ds_affixes.cpp",
                    555,
                    empty_string);
        }
        pos = lineEnd + 1;
    }
}

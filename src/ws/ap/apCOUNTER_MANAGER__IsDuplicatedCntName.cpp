#include "apCOUNTER_MANAGER.h"
#include "apCOUNTER.h"
#include "../ds/dsTSTRING_BUF_HEADER.h"

// apCOUNTER_MANAGER::IsDuplicatedCntName @ 0x82716670 — const (ABA mangle), returns bool (_N).
// True when `name` already matches the name of a registered counter. A shared buffer pointer is a
// fast-path match; otherwise the lengths and then the bytes are compared.
bool apCOUNTER_MANAGER::IsDuplicatedCntName(const dsTSTRING<char> *name) const
{
    if (this->counters.nElem <= 0)
        return false;

    for (int i = 0; ; ++i)
    {
        apCOUNTER *counter = this->counters[i];
        dsTSTRING_BUF_HEADER<char> *nameBuffer = name->pBuffer;
        dsTSTRING_BUF_HEADER<char> *cntBuffer = counter->name.pBuffer;

        bool match;
        if (nameBuffer == cntBuffer)
        {
            match = true;
        }
        else
        {
            int nameLen = nameBuffer->strLen;
            if (nameLen != cntBuffer->strLen)
            {
                match = false;
            }
            else if (nameLen == 0)
            {
                match = true;
            }
            else
            {
                const char *cntChars  = cntBuffer->str;
                const char *nameChars = nameBuffer->str;
                const unsigned char *nameEnd = (const unsigned char *)&nameChars[nameLen];
                int diff;
                do
                {
                    int cntChar = (unsigned char)*cntChars;
                    diff = (unsigned char)*nameChars - cntChar;
                    if ((unsigned char)*nameChars != cntChar)
                        break;
                    ++nameChars;
                    ++cntChars;
                }
                while (nameChars != (const char *)nameEnd);
                match = (diff == 0);
            }
        }

        if (match)
            return true;
        if (i + 1 >= this->counters.nElem)
            return false;
    }
}

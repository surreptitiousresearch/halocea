#include "dbgVAR_MANAGER.h"

// dbgVAR_MANAGER::FindFirst @ 0x8255FB00
// Linear scan for the first registered variable whose name exactly equals `name`: a shared-buffer
// pointer match is a fast path, otherwise a length-then-byte-by-byte compare (mirrors
// dsTSTRING<char>::operator==, open-coded here rather than calling it).
dbgVAR *dbgVAR_MANAGER::FindFirst(const dsTSTRING<char> &name)
{
    this->lock.Lock(nullptr, 0);

    for (int index = 0; index < this->vars.nElem; ++index)
    {
        dbgVAR *candidate = this->vars[index];
        dsTSTRING_BUF_HEADER<char> *candidateBuf = candidate->name.pBuffer;
        dsTSTRING_BUF_HEADER<char> *nameBuf = name.pBuffer;
        bool matches;

        if (candidateBuf == nameBuf)
        {
            matches = true;
        }
        else if (candidateBuf->strLen != nameBuf->strLen)
        {
            matches = false;
        }
        else if (candidateBuf->strLen == 0)
        {
            matches = true;
        }
        else
        {
            const char *nameStr = nameBuf->str;
            const char *candidateStr = candidateBuf->str;
            const char *candidateEnd = candidateStr + candidateBuf->strLen;
            int diff = 0;

            do
            {
                diff = (unsigned char)*candidateStr - (unsigned char)*nameStr;
                if ((unsigned char)*candidateStr != (unsigned char)*nameStr)
                    break;
                ++candidateStr;
                ++nameStr;
            } while (candidateStr != candidateEnd);

            matches = (diff == 0);
        }

        if (matches)
        {
            dbgVAR *found = this->vars[index];
            this->lock.Unlock(nullptr, 0);
            return found;
        }
    }

    this->lock.Unlock(nullptr, 0);
    return nullptr;
}

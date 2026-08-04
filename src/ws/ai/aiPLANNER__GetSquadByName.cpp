#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiSQUAD.h"

extern void dlFree(void *ptr);

// aiPLANNER::GetSquadByName(const dsTSTRING<char>&) — 0x83166ED8
// Linear scan of arrSquad, comparing each squad's GetName() against squadName. The comparison is
// dsTSTRING's inlined equality: same buffer pointer, else equal length + byte-wise compare.
// GetName() returns a temporary dsTSTRING by value (its buffer refcount is bumped on return and
// dropped here after the compare), matching the release seen at 0x83166FB4.
aiSQUAD *aiPLANNER::GetSquadByName(const dsTSTRING<char> &squadName)
{
    int nElem = arrSquad.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        aiSQUAD *squad = arrSquad[i];

        dsTSTRING<char> name = squad->GetName();

        dsTSTRING_BUF_HEADER<char> *lhs = name.pBuffer;
        dsTSTRING_BUF_HEADER<char> *rhs = squadName.pBuffer;

        bool equal;
        if (lhs == rhs)
        {
            equal = true;
        }
        else if (lhs->strLen != rhs->strLen)
        {
            equal = false;
        }
        else if (lhs->strLen == 0)
        {
            equal = true;
        }
        else
        {
            const char *a = rhs->str;
            const char *b = lhs->str;
            const char *bEnd = &lhs->str[lhs->strLen];
            int diff = 0;
            do
            {
                diff = (unsigned char)*b - (unsigned char)*a;
                if (diff != 0)
                    break;
                ++b;
                ++a;
            } while (b != bEnd);
            equal = (diff == 0);
        }

        // Drop the reference held by the returned-by-value name.
        if (--name.pBuffer->refCount == 0)
            dlFree(name.pBuffer);

        if (equal)
            return arrSquad[i];
    }
    return nullptr;
}

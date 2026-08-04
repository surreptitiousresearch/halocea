#include "apCOUNTER.h"
#include "apCOUNTER_MANAGER.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc free

namespace ds {
// ds::DEF_TOKEN_PROCESSOR — default '/'-delimited token collector used by ds::Tokenize; appends
// each token found into a caller-supplied dsVECTOR<dsTSTRING<char>,8>. Boundary — not reversed here.
struct DEF_TOKEN_PROCESSOR;
// ds::Tokenize<ds::DEF_TOKEN_PROCESSOR> — split `text` on occurrences of `delim`, feeding each
// token to `processor`. Boundary — not reversed here.
template<class PROCESSOR>
void Tokenize(dsTSTRING<char> *text, dsTSTRING<char> *delim, PROCESSOR *processor);
}

// apCOUNTER::apCOUNTER @ 0x82719D00
// Construct with `_name` and a '/'-delimited `unitsNames` list (e.g. "calls", "time/calls"):
// tokenize `unitsNames` on '/' into a scratch dsVECTOR<dsTSTRING<char>,8>, resize `counterUnits`
// to the token count, then move each token's shared string buffer into the matching unit's
// `name` (releasing the unit's prior buffer, adopting the token's buffer and bumping its
// refcount) -- finally register with the apCOUNTER_MANAGER singleton.
apCOUNTER::apCOUNTER(const char *_name, const char *unitsNames)
{
    this->__vftable = (apCOUNTER_vtbl *)&apCOUNTER::vftable;
    this->name.pBuffer = nullptr;
    this->name.UnsafeInit(_name, -1, 0);
    this->counterUnits.pData = nullptr;
    this->counterUnits.nElem = 0;
    this->counterUnits.allocated = 0;

    dsVECTOR<dsTSTRING<char>, 8> tokens;
    tokens.pData = nullptr;
    tokens.nElem = 0;
    tokens.allocated = 0;

    dsTSTRING<char> delim;
    delim.pBuffer = nullptr;
    delim.UnsafeInit("/", -1, 0);

    dsTSTRING<char> unitsText;
    unitsText.pBuffer = nullptr;
    unitsText.UnsafeInit(unitsNames, -1, 0);

    ds::Tokenize<ds::DEF_TOKEN_PROCESSOR>(&unitsText, &delim, (ds::DEF_TOKEN_PROCESSOR *)&tokens);

    if (--unitsText.pBuffer->refCount == 0)
        dlFree(unitsText.pBuffer);
    if (--delim.pBuffer->refCount == 0)
        dlFree(delim.pBuffer);

    int tokenCount = tokens.nElem;
    this->counterUnits.Resize(tokenCount);

    for (int i = 0; i < tokenCount; ++i)
    {
        dsTSTRING<char> &token = tokens[i];
        apCOUNTER_UNIT &unit = this->counterUnits[i];

        if (&token != (dsTSTRING<char> *)&unit)
        {
            dsTSTRING_BUF_HEADER<char> *oldBuffer = unit.name.pBuffer;
            if (--oldBuffer->refCount == 0)
                dlFree(oldBuffer);

            unit.name.pBuffer = token.pBuffer;
            ++token.pBuffer->refCount;
        }
    }

    this->id = apCOUNTER_MANAGER::Instance().AddCounter(this) << 8;

    // `tokens` (scratch dsVECTOR<dsTSTRING<char>,8>) goes out of scope here and its destructor
    // releases the token buffers -- matches the decompile's explicit dtor call at 0x82719E90.
}

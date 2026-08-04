#include "dsAFFIX_BUILDER.h"

// dsAFFIX_BUILDER::AFFIX::AFFIX(const AFFIX&) @ 0x82A778D0
// Copy-construct one affix from another: share the source name buffer (bump its refcount) and
// deep-copy the parameter vector by splicing the source elements into a freshly-empty vector.
dsAFFIX_BUILDER::AFFIX::AFFIX(const dsAFFIX_BUILDER::AFFIX &that)
{
    this->name.pBuffer = nullptr;
    dsTSTRING_BUF_HEADER<char> *sharedNameBuffer = that.name.pBuffer;
    this->name.pBuffer = that.name.pBuffer;
    ++sharedNameBuffer->refCount;

    this->params.pData = nullptr;
    this->params.nElem = 0;
    this->params.allocated = 0;
    this->params.__cl = that.params.__cl;
    this->params.Insert(0, that.params.pData, that.params.nElem);
}

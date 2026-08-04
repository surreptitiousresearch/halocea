#include "dsAFFIX_BUILDER.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsAFFIX_BUILDER::AFFIX::operator= @ 0x82A77968
// Copy-assign one affix onto another: release this affix name buffer, adopt (share) the
// source name buffer bumping its refcount, then deep-copy the parameter vector.
dsAFFIX_BUILDER::AFFIX &dsAFFIX_BUILDER::AFFIX::operator=(const dsAFFIX_BUILDER::AFFIX &that)
{
    if (&that != this) {
        dsTSTRING_BUF_HEADER<char> *oldNameBuffer = this->name.pBuffer;
        if (oldNameBuffer->refCount-- == 1)
            dlFree(oldNameBuffer);
        dsTSTRING_BUF_HEADER<char> *newNameBuffer = that.name.pBuffer;
        this->name.pBuffer = that.name.pBuffer;
        ++newNameBuffer->refCount;
    }
    this->params = that.params; // dsVECTOR<AFF_PARAM,8>::operator= (boundary)
    return *this;
}

// dsPAIR<dsTSTRING<char>,dsVECTOR<hcexCINE_INFO,8>>::dsPAIR(const dsPAIR &that) @ 0x823B8080
//
// Copy-construct one listCine entry. The compiler inlined both member copy-constructors: the key
// dsTSTRING<char> shares `that`'s buffer (bumping its refcount), and the value
// dsVECTOR<hcexCINE_INFO,8> starts empty (null storage, adopting that's call-site cookie) and
// Inserts the whole source range. Reversed per-instantiation.

#include "../headers/hcex/hcexCINE_INFO.h"
#include "../headers/ws/ds/dsVECTOR.h"
#include "../headers/ws/ds/dsTSTRING.h"
#include "../headers/ws/ds/dsPAIR.h"

typedef dsPAIR<dsTSTRING<char>, dsVECTOR<hcexCINE_INFO, 8> > hcex_cine_entry;

template<>
hcex_cine_entry::dsPAIR(const hcex_cine_entry &that)
{
    // inlined dsTSTRING<char> copy-ctor: adopt the shared buffer, bump its refcount.
    this->key.pBuffer = that.key.pBuffer;
    ++this->key.pBuffer->refCount;

    // inlined dsVECTOR<hcexCINE_INFO,8> copy-ctor: empty, adopt cookie, then Insert source range.
    this->val.pData = nullptr;
    this->val.nElem = 0;
    this->val.allocated = 0;
    this->val.__cl = that.val.__cl;
    this->val.Insert(0, that.val.pData, that.val.nElem);
}

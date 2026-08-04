#include "dsVECTOR.h"
#include "dsTSTRING.h"
#include "../../apCL.h"

// dsVECTOR<dsTSTRING<char>,8>::dsVECTOR(const apCL&) @ 0x823B2F78
// Empty-construct: null storage, zero count, record the call-site cookie.
template<>
dsVECTOR<dsTSTRING<char>, 8>::dsVECTOR(const apCL &cl)
{
    this->__cl = cl;
    this->pData = nullptr;
    this->nElem = 0;
    this->allocated = 0;
}

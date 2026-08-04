#include "dsVECTOR.h"
#include "dsTSTRING.h"
#include "../../apCL.h"

// dsVECTOR<dsTSTRING<char>,8>::dsVECTOR (copy) @ 0x823DFFC8
template<>
dsVECTOR<dsTSTRING<char>, 8>::dsVECTOR(const dsVECTOR<dsTSTRING<char>, 8> &src)
{
    this->pData = nullptr;
    this->nElem = 0;
    this->allocated = 0;
    this->__cl = src.__cl;
    this->Insert(0, src.pData, src.nElem);
}

#include "dsSTATIC_VECTOR.h"
#include "dsSTRID.h"

// dsSTATIC_VECTOR<dsSTRID,20>::Back @ 0x825160D0
// Reference to the last live element; assert the vector is non-empty first.
// The address math `(char*)this + 4*nElem` is &pData[nElem-1] (nElem@0, pData@4, sizeof dsSTRID==4).
template<>
dsSTRID &dsSTATIC_VECTOR<dsSTRID, 20>::Back()
{
    if (!IGNORE_STRONG_ASSERT && this->nElem <= 0)
        reinterpret_cast<STRONG_ASSERT_DUMMY *>(0)->Crash(
            "nElem > 0",
            "D:\\Projects\\code\\common\\src.sys\\ps\\ps_section.cpp",
            28,
            empty_string);
    return this->pData[this->nElem - 1];
}

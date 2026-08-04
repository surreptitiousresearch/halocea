#include "dsSTATIC_VECTOR.h"
#include "dsSTRID.h"
#include "ds_assert_boundary.h"

// dsSTATIC_VECTOR<dsSTRID,20>::PushBack @ 0x82516148
// Append a copy of `val` to the inline storage (asserting capacity first) and return a
// reference to the freshly appended element.
template<>
dsSTRID &dsSTATIC_VECTOR<dsSTRID, 20>::PushBack(const dsSTRID &val)
{
    if (!IGNORE_STRONG_ASSERT && this->nElem >= 20)
        reinterpret_cast<STRONG_ASSERT_DUMMY *>(0)->Crash(
            "nElem < size",
            "D:\\Projects\\code\\common\\src.sys\\ps\\ps_section.cpp",
            29,
            dsStrongAssertMessage);
    this->pData[this->nElem] = val;
    ++this->nElem;
    return this->pData[this->nElem - 1];
}

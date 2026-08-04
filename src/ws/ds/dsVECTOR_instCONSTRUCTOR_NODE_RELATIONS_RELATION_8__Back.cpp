#include "dsVECTOR.h"
#include "../prop/instCONSTRUCTOR_NODE_RELATIONS.h"
#include "ds_assert_boundary.h"

// dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS::RELATION,8>::Back @ 0x826DEF08
// Reference to the last live element (index nElem-1); assert non-empty first.
template<>
instCONSTRUCTOR_NODE_RELATIONS::RELATION &dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS::RELATION, 8>::Back()
{
    if (!IGNORE_STRONG_ASSERT && this->nElem <= 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "nElem > 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            527,
            "nElem",
            this->nElem);
    return this->pData[this->nElem - 1];
}

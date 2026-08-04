#include "dsNODE_CACHE.h"
#include "dsMsgAddrMapDUMMY.h"

void operator delete[](void *p); // boundary — plain array delete (no debug cookie overload here)

// dsNODE_CACHE<dsMsgAddrMapDUMMY>::Term @ 0x8255A8F0
// Clear the free list (drops all links; the nodes themselves live inside the owned blocks below),
// `operator delete[]` every owned node block, drop nodeList's live count to 0, shrink its backing
// storage to match (Realloc(0)), and reset nNode to 0.
// DEVIATION: the decompile's second loop (`for(j=0;j<nElem;++j) ;`) has an empty body — this is the
// dead remnant of the same "advance nElem to 0 without touching pData" pattern the sibling
// dsNODE_CACHE::Term reversals show (the loop itself does nothing; only the `nElem = 0` after it
// matters). Reproduced faithfully as a no-op loop.
template<>
void dsNODE_CACHE<dsMsgAddrMapDUMMY>::Term()
{
    freeNodeList.Clear();

    for (int i = 0; i < nodeList.nElem; ++i) {
        void *block = nodeList[i];
        ::operator delete[](block);
    }

    for (int j = 0; j < nodeList.nElem; ++j)
        ; // no-op (see DEVIATION note above)

    nodeList.nElem = 0;
    nodeList.Realloc(nodeList.nElem);
    nNode = 0;
}

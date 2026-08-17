/* IDirect3DQuery9_Issue @0x83785C28 */
#include "headers/d3d_boundary.h"
extern void D3DQuery_Issue(D3DQuery *query, unsigned int flags);

HRESULT IDirect3DQuery9_Issue(D3DQuery *pThis, unsigned int IssueFlags)
{
    D3DQuery_Issue(pThis, IssueFlags);
    return 0;
}

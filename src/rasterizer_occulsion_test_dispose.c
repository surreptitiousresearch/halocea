/* rasterizer_occulsion_test_dispose @0x83785EE8 — release every occlusion query.
 * DEVIATION: the decompiler bounds the loop with `&widget_dxeffect_shader`, an address that happens to
 * coincide with the end of occlusion_query[1024] due to link-time global layout; restored to a clean
 * indexed loop over the array. */

#include "headers/rasterizer_occlusion_globals.h"
#include <stddef.h>   /* NULL */

extern unsigned int D3DQuery_Release(D3DQuery *pThis);

D3DQuery *rasterizer_occulsion_test_dispose(void)
{
    D3DQuery *result = NULL;

    for (int i = 0; i < 1024; ++i)
    {
        if (occlusion_query[i])
        {
            result = (D3DQuery *)D3DQuery_Release(occlusion_query[i]);
            occlusion_query[i] = NULL;
        }
    }

    return result;
}

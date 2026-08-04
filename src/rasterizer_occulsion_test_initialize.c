/* rasterizer_occulsion_test_initialize @ 0x83785E28
   Creates one tiled D3D occlusion query per slot in occlusion_query[]. Stops
   on the first failure. The array end is computed as the address of the
   adjacent global widget_dxeffect_shader (the two are contiguous in the data
   segment, giving exactly 1024 slots). */

#include <stdint.h>
#include "headers/rasterizer_occlusion_globals.h"
#include "headers/rasterizer_engine_globals.h"

extern void *memset(void *dst, int value, unsigned int size);

uint8_t rasterizer_occulsion_test_initialize(void) /* re-attested: extrwi bool + both callers clrlwi24 */
{
    uint8_t succeeded = 1;
    supports_occlusion_test = 1;
    memset(occlusion_query, 0, sizeof(occlusion_query));

    for ( D3DQuery **slot = occlusion_query;
          slot < (D3DQuery **)&widget_dxeffect_shader && succeeded; /* raw address compare, 0x83785EC4 */
          ++slot )
    {
        D3DQuery *query = D3DDevice_CreateQueryTiled(global_d3d_device, D3DQUERYTYPE_OCCLUSION, 1);
        /* Binary synthesizes an HRESULT from the returned pointer:
           NULL -> E_OUTOFMEMORY (0x8007000E), else S_OK (0x83785E8C..0x83785E94). */
        int32_t create_result = (query != 0) ? 0 : (int32_t)0x8007000E;
        if ( create_result >= 0 )
        {
            *slot = query;
        }
        else
        {
            succeeded = 0;
            /* Preserved from binary (0x83785EA4): with the synthesized HRESULT above this
               D3DERR_NOTAVAILABLE check can never hit, but the compare exists in the code. */
            if ( create_result == (int32_t)0x8876086A )
                supports_occlusion_test = 0;
        }
    }
    return succeeded == supports_occlusion_test;
}

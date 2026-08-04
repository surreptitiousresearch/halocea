#include "headers/d3d_boundary.h"
#include "headers/blam_data_globals.h"


/* DEVIATION: State is a byte offset into the sampler-state call table at device+0x1D4 */
void SetSamplerStageStateSmart(unsigned int stage, unsigned int State, unsigned int Value)
{
    D3DDevice *device = global_d3d_device;
    /* DEVIATION: single-level function pointer; call directly (was double-ptr, uncallable). */
    void (*pfn)(D3DDevice *, unsigned int, unsigned int) =
        *(void (**)(D3DDevice *, unsigned int, unsigned int))((char *)device + 0x1D4 + State);
    pfn(device, stage, Value);
}

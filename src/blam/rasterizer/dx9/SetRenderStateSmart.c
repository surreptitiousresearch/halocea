/* SetRenderStateSmart @0x83685A10 */
#include "headers/d3d_boundary.h"
#include "headers/blam_data_globals.h"


void SetRenderStateSmart(_D3DRENDERSTATETYPE State, unsigned int Value)
{
    global_d3d_device->m_SetRenderStateCall[State](global_d3d_device, Value);
}

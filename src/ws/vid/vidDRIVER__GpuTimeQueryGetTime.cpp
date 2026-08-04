// vidDRIVER::GpuTimeQueryGetTime @0x8266B9B8 — base-class virtual stub. The generic video driver
// has no hardware GPU timestamp queries, so a timed-pass read always yields 0; the d3dDRIVER
// override (@0x82692270) issues the real D3D timestamp query. Args match the override: the timed
// pass id and the split index (eye/tile) whose elapsed time is requested.
#include "../../headers/vidDRIVER.h"

unsigned __int64 vidDRIVER::GpuTimeQueryGetTime(VID_TQ query, int split_index)
{
    (void)query;
    (void)split_index;
    return 0;
}

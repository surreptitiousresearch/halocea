// vidDRIVER::GpuTimeQueryGetFreq @0x8266B9C0 — base-class virtual stub returning a timer frequency
// of 1 (so callers dividing elapsed-ticks by frequency get a defined, harmless result). The
// d3dDRIVER override (@0x826922B8) reports the real GPU timestamp frequency.
#include "../../headers/vidDRIVER.h"

unsigned __int64 vidDRIVER::GpuTimeQueryGetFreq()
{
    return 1;
}

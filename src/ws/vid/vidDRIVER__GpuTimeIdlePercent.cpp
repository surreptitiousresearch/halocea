// vidDRIVER::GpuTimeIdlePercent @0x8266B9C8 — base-class virtual stub reporting 0% GPU idle time.
// The d3dDRIVER override (@0x826922F8) derives the real idle fraction from the frame's timestamp
// queries.
#include "../../headers/vidDRIVER.h"

float vidDRIVER::GpuTimeIdlePercent()
{
    return 0.0f;
}

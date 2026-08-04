#include "../../headers/ws/ent/entSFX_COLOR.h"

// entSFX_COLOR::implSetIntensity @ 0x82707F28 — force the colorizer's animation phase directly by
// storing `intensity` into forceAnimTime (the effect samples its color ramp at this time value).
void entSFX_COLOR::implSetIntensity(float intensity)
{
    this->forceAnimTime = intensity;
}

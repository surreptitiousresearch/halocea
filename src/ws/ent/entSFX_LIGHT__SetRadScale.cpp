#include "../../headers/ws/ent/entSFX_LIGHT.h"
#include "../../headers/ws/rend/rendLIGHT.h"
#include "../../headers/ws/lgt/lgtLIGHT.h"

// entSFX_LIGHT::SetRadScale @ 0x826F2A90 — write the radius scale into the bound runtime light's
// data record (rendLIGHT::lightData->scale, lgtLIGHT @ 0xB8). No-op when no light is bound.
void entSFX_LIGHT::SetRadScale(float scale)
{
    rendLIGHT *light = this->pLight.pointee;
    if (light)
        light->lightData->scale = scale;
}

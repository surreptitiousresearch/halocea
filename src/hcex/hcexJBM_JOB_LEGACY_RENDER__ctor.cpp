/* hcexJBM_JOB_LEGACY_RENDER::hcexJBM_JOB_LEGACY_RENDER @0x823F08C8 — default-construct the
 * legacy-render job: set the debug name "legacy_render", install this class's own vtable, and
 * clear the queued/running state. jbmJOB adds no other storage (size 12). */

#include "../headers/hcex/hcexJBM_JOB_LEGACY_RENDER.h"

hcexJBM_JOB_LEGACY_RENDER::hcexJBM_JOB_LEGACY_RENDER()
{
    this->name = "legacy_render";
    this->__vftable = (jbmJOB_vtbl *)&hcexJBM_JOB_LEGACY_RENDER::vftable; // base __vftable type
    this->state.state = 0;
}

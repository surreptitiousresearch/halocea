/* hcexHALO_MODEL::hcexHALO_MODEL @0x823D33F0 — install this class's vftable (no other state to
 * init). */

#include "../../headers/hcex/hcexHALO_MODEL.h"

hcexHALO_MODEL_vtbl hcexHALO_MODEL::vftable; // boundary storage — real contents not modeled

hcexHALO_MODEL::hcexHALO_MODEL()
{
    __vftable = reinterpret_cast<entMANAGER_HANDLER_vtbl *>(&hcexHALO_MODEL::vftable);
}

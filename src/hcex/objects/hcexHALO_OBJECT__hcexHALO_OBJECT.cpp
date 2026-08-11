/* hcexHALO_OBJECT::hcexHALO_OBJECT @0x823D1238 — install this class's vftable (no other state to
 * init; entMANAGER_HANDLER's own ctor already ran as the base subobject). */

#include "../../headers/hcex/hcexHALO_OBJECT.h"

hcexHALO_OBJECT_vtbl hcexHALO_OBJECT::vftable; // boundary storage — real contents not modeled

hcexHALO_OBJECT::hcexHALO_OBJECT()
{
    __vftable = reinterpret_cast<entMANAGER_HANDLER_vtbl *>(&hcexHALO_OBJECT::vftable);
}

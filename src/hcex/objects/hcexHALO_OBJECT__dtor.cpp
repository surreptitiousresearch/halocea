/* hcexHALO_OBJECT::~hcexHALO_OBJECT @0x823CEBD0 — reset __vftable to the entMANAGER_HANDLER base
 * slot (no owned state to release). */

#include "../../headers/hcex/hcexHALO_OBJECT.h"

hcexHALO_OBJECT::~hcexHALO_OBJECT()
{
    __vftable = &entMANAGER_HANDLER::vftable;
}

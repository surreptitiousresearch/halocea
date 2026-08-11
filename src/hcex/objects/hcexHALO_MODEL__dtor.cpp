/* hcexHALO_MODEL::~hcexHALO_MODEL @0x823D1228 — reset __vftable to the entMANAGER_HANDLER base
 * slot (no owned state to release). */

#include "../../headers/hcex/hcexHALO_MODEL.h"

hcexHALO_MODEL::~hcexHALO_MODEL()
{
    __vftable = &entMANAGER_HANDLER::vftable;
}

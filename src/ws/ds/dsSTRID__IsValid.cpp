// dsSTRID::IsValid @0x825281A0 — true when `id` is not the string pool's reserved empty-id.
#include "../../headers/ws/ds/dsSTRID.h"

extern const char *dsStrIdOfEmptyId;

bool dsSTRID::IsValid() const
{
    return id != dsStrIdOfEmptyId;
}

#pragma once
// wpnWEAPON is defined once (canonically) in wpnLIST_BASE_boundary.h: DB-verified size 260,
// propFSM base @0x00, pOwner @0x0C (iaIACTOR *, via propBASE), shooter @0x9C. This header used to
// carry a competing opaque copy that mistyped pOwner as entENTITY *; per the DB (propBASE.pOwner is
// iaIACTOR *) that was wrong — entENTITY derives from iaIACTOR, so callers needing entENTITY
// downcast pOwner. Kept as a thin alias so existing includers (aiWEAPON::SwitchLaser) still resolve.
#include "wpnLIST_BASE_boundary.h"

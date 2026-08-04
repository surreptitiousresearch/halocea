#pragma once
#include "hkMatrix3.h"

/* hkRotation (48 bytes, DB types_members: hkMatrix3 base only) — layout-identical
   to hkMatrix3 (three column hkVector4). The engine spells it `hkRotation :
   hkMatrix3`; that inheritance syntax is invalid under the C compile used for the
   reconstructed TUs, so provide a flat, layout-identical struct there. */
#ifdef __cplusplus
struct hkRotation : hkMatrix3
{
};
typedef struct hkRotation hkRotation;
#else
/* Under C there is no inheritance and hkRotation adds no members of its own (DB types_members
   lists a single hkMatrix3 base and nothing else), so alias it rather than duplicating the
   layout as a *distinct* type. Duplicating it is what made every `hkRotation *` -> `hkMatrix3 *`
   upcast — legal and implicit in the original C++ — a C4133 in the reconstructed C TUs. */
typedef hkMatrix3 hkRotation;
#endif

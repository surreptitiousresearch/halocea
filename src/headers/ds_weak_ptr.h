#pragma once
/* ds::WEAK_PTR<entENTITY> — refcounted indirection to an entity; pPtr is null once the entity is
 * gone. Now aliases of the canonical ds::WEAK_PTR<T> / ds::WEAK_PTR_HANDLE<T> templates
 * (DB dsWEAK_PTR family; identical layout). */

#include "ws/ds/WEAK_PTR.h"

typedef struct entENTITY entENTITY;

typedef ds::WEAK_PTR_HANDLE<entENTITY> ds_WEAK_PTR_HANDLE;
typedef ds::WEAK_PTR<entENTITY> ds_WEAK_PTR;

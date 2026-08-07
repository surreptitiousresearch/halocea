#pragma once
/* apProfileTimer lives in the ws `ap` (platform-abstraction) tree, next to the TU that defines
 * GetResSec. This root-level header used to carry a second file-scope copy of the body spelled
 * with `unsigned long long`; forward to the canonical one instead.
 *
 * The include is path-qualified on purpose: the basename `apProfileTimer.h` exists at two paths,
 * so a bare `#include "apProfileTimer.h"` would resolve by include-directory order, not intent.
 * Both paths now name the same single definition either way. */

#include "ws/ap/apProfileTimer.h"

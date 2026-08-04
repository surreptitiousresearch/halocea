#pragma once
#include "../ssl/sslDESC.h"
// ws-engine ui_new: descriptor base for a UI_FRAME-derived class, adding the lazy
// "has this SSL script class been initialized yet" flag on top of sslDESC.
// DB-verified layout (types_members ui_new::UI_FRAME_DESC): sslDESC (base)@0 (20B),
// isSslClassInited@20 (DEFAULT_CTOR<bool>, 1B) — size 24 (padded).
#include "../DEFAULT_CTOR.h"

namespace ui_new {

struct UI_FRAME_DESC : sslDESC {
    DEFAULT_CTOR<bool> isSslClassInited; // 0x14
};

} // namespace ui_new

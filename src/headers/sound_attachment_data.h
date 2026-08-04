/* sound_attachment_data — describes where on an object a sound is attached: which node, and the
 * local-space position/forward to transform by that node's matrix (28 bytes). */
#pragma once

#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct sound_attachment_data
{
    __int16       function_index; // 0x00
    __int16       node_index;     // 0x02
    real_point3d  position;       // 0x04
    real_vector3d forward;        // 0x10
} sound_attachment_data;

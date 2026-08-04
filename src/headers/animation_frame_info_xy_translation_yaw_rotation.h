#ifndef ANIMATION_FRAME_INFO_XY_TRANSLATION_YAW_ROTATION_H
#define ANIMATION_FRAME_INFO_XY_TRANSLATION_YAW_ROTATION_H

#include "real_vector2d.h"

/* DB-verified: types_members animation_frame_info_xy_translation_yaw_rotation (12 bytes). */
typedef struct animation_frame_info_xy_translation_yaw_rotation
{
    real_vector2d offset; /* 0x00 */
    float         yaw;    /* 0x08 */
} animation_frame_info_xy_translation_yaw_rotation; /* 0x0C */

#endif /* ANIMATION_FRAME_INFO_XY_TRANSLATION_YAW_ROTATION_H */

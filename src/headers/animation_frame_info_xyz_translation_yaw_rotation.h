#ifndef ANIMATION_FRAME_INFO_XYZ_TRANSLATION_YAW_ROTATION_H
#define ANIMATION_FRAME_INFO_XYZ_TRANSLATION_YAW_ROTATION_H

#include "real_vector3d.h"

/* DB-verified: types_members animation_frame_info_xyz_translation_yaw_rotation (16 bytes). */
typedef struct animation_frame_info_xyz_translation_yaw_rotation
{
    real_vector3d offset; /* 0x00 */
    float         yaw;    /* 0x0C */
} animation_frame_info_xyz_translation_yaw_rotation; /* 0x10 */

#endif /* ANIMATION_FRAME_INFO_XYZ_TRANSLATION_YAW_ROTATION_H */

#pragma once
// DB-verified via types_members _49197FBDBC1AD3D1BAA83ADB63B3EA44 (anonymous PDB type)
/* camera_script_globals — runtime state of the scripted (cinematic) camera. DB layout (anonymous struct). */

#include <stdint.h>
#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct camera_script_globals
{
    unsigned char enabled;                 /* 0x00 */
    unsigned char first_update;            /* 0x01 */
    int16_t       mode;                    /* 0x02 — 0 scripted point, 2 first-person, 3 dead-cam */
    int16_t       camera_point_index;      /* 0x04 */
    unsigned char _pad06[2];               /* 0x06 */
    float         timer;                   /* 0x08 — remaining time in seconds */
    real_point3d  point;                   /* 0x0C */
    real_vector3d forward;                 /* 0x18 */
    real_vector3d up;                      /* 0x24 */
    float         field_of_view;           /* 0x30 */
    int           relative_object_index;   /* 0x34 */
    int           animation_graph_index;   /* 0x38 */
    int16_t       animation_index;         /* 0x3C */
} camera_script_globals_t;                 /* 0x3E */

#ifdef __cplusplus
extern "C" {
#endif

extern camera_script_globals_t camera_script_globals;

#ifdef __cplusplus
}
#endif

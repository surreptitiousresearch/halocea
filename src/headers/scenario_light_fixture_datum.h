#pragma once
/* scenario_light_fixture_datum — scenario placement block for a light fixture object (DB
 * types_members-confirmed, 88 bytes). Consumed by light_fixture_place.c to copy the tag's light
 * parameters onto the placed object's runtime light-data fields. */

#include "scenario_object_datum.h"
#include "scenario_device_datum.h"
#include "real_rgb_color.h"

typedef struct scenario_light_fixture_datum
{
    scenario_object_datum object;        /* 0x00 */
    scenario_device_datum device;        /* 0x28 */
    real_rgb_color         color;         /* 0x30 */
    float                  intensity;     /* 0x3C */
    float                  falloff_angle; /* 0x40 */
    float                  cutoff_angle;  /* 0x44 */
    unsigned int           unused[4];     /* 0x48 */
} scenario_light_fixture_datum; /* 88 bytes */

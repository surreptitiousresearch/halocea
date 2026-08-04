#pragma once
/* first_person_camera — persistent state for the first-person camera proc. Only the previous
 * field-of-view is retained, to detect FOV changes and trigger a smooth FOV interpolation. */

typedef struct first_person_camera
{
    float last_field_of_view;   /* 0x0 */
} first_person_camera;          /* 4 bytes */

#pragma once
/* spherical_coordinate_vector — a direction expressed as (theta, phi) angles (8 bytes). DB layout. */

typedef struct spherical_coordinate_vector
{
    float theta; /* 0x00 */
    float phi;   /* 0x04 */
} spherical_coordinate_vector;

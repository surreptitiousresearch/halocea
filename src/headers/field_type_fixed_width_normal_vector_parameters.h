#pragma once
/* _field_type_fixed_width_normal_vector_parameters — the `parameters` blob for a fixed-width normal-vector
 * field (16 bytes, DB layout): separate internet/LAN bit budgets for theta/phi. */

typedef struct _field_type_fixed_width_normal_vector_parameters
{
    const int number_of_bits_theta_internet; /* 0x00 */
    const int number_of_bits_phi_internet;   /* 0x04 */
    const int number_of_bits_theta_lan;      /* 0x08 */
    const int number_of_bits_phi_lan;        /* 0x0C */
} _field_type_fixed_width_normal_vector_parameters;

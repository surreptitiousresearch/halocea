#pragma once
/* _field_type_smart_vector_parameters — tuning for the "smart vector" quantized-vector message-delta field
 * type (DB types_members-confirmed, 796 bytes). Each component is quantized against a bit-width/range pair
 * that differs by network transport: LAN gets the wider (higher-precision) range, internet the narrower one,
 * selected at encode/decode time by gMessageEncodingClass. */

#include "real_vector3d.h"
#include "quantized_smart_vector.h"

typedef struct _field_type_smart_vector_parameters
{
    const float                 min_value;                              /* 0x000 */
    const float                 max_value;                              /* 0x004 */
    const int                   number_of_bits_per_component_internet;  /* 0x008 */
    int                         range_of_values_per_component_internet; /* 0x00C */
    const int                   number_of_bits_per_component_lan;       /* 0x010 */
    int                         range_of_values_per_component_lan;      /* 0x014 */
    const int                   number_of_common_vectors;                /* 0x018 */
    const real_vector3d         common_vectors[32];                      /* 0x01C */
    quantized_smart_vector      squished_common_vectors[32];             /* 0x19C */
} _field_type_smart_vector_parameters; /* 796 bytes */

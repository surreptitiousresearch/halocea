#pragma once
/* _field_type_vector_parameters — the `parameters` blob for a "vector" message-delta field (4 bytes,
 * DB layout, confirmed by headers_ref `_field_type_vector_parameters` sizeof=0x4): how many real
 * components the vector carries. Distinct type from _field_type_point_parameters despite the same
 * layout — the two field types have separate encoders and separate size calculators. */

typedef struct _field_type_vector_parameters
{
    const int dimensions;   /* 0x00 */
} _field_type_vector_parameters;

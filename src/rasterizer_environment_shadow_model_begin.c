/* rasterizer_environment_shadow_model_begin 0x836A61E0 — argument-forwarding tail-call thunk to
 * _rasterizer_environment_shadow_model_begin. */

#include "headers/rasterizer_model_begin_parameters.h"

extern void _rasterizer_environment_shadow_model_begin(const rasterizer_model_begin_parameters *parameters);

void rasterizer_environment_shadow_model_begin(const rasterizer_model_begin_parameters *parameters)
{
    _rasterizer_environment_shadow_model_begin(parameters);
}

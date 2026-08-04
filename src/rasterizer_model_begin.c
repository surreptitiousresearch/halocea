/* rasterizer_model_begin 0x836A6128 — argument-forwarding tail-call thunk to _rasterizer_model_begin. */

#include <stdint.h>
#include "headers/rasterizer_model_begin_parameters.h"

extern void _rasterizer_model_begin(const rasterizer_model_begin_parameters *parameters, uint8_t do_not_change_z_stencil_states);

void rasterizer_model_begin(const rasterizer_model_begin_parameters *parameters, uint8_t do_not_change_z_stencil_states)
{
    _rasterizer_model_begin(parameters, do_not_change_z_stencil_states);
}

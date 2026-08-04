#include <stdint.h>
#include "headers/real_matrix4x3.h"

extern void _rasterizer_model_setupnodeparts(int num_nodes, const uint8_t *node_table, real_matrix4x3 *node_matrices);

void rasterizer_model_setupnodeparts(int num_nodes, const uint8_t *node_table, real_matrix4x3 *node_matrices)
{
    _rasterizer_model_setupnodeparts(num_nodes, node_table, node_matrices);
}

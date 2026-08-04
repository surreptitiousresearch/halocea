#include <stdint.h>
#include "headers/model.h"
#include "headers/model_node.h"
#include "headers/real_matrix4x3.h"

const real_matrix4x3 * model_get_default_inverse_matrix(const model *model, int16_t node_index)
{
    return &((const model_node *)model->nodes.address)[node_index].runtime_default_inverse_matrix;
}

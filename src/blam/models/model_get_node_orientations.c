/* model_get_node_orientations @ 0x83798CA0 — fill node_orientations[] with each model node's
 * default (base-pose) orientation taken straight from the model tag's node block. Per node this
 * copies the default rotation quaternion (node floats[13..16]) and default translation
 * (node floats[10..12]) and sets scale to 1. node_orientations must hold model->nodes.count
 * elements. Node stride is 156 bytes. */

#include "headers/model.h"
#include "headers/model_node.h"
#include "headers/real_orientation.h"

void model_get_node_orientations(const model *model, real_orientation *node_orientations)
{
    for ( int i = 0; i < model->nodes.count; ++i )
    {
        const model_node *node = &((const model_node *)model->nodes.address)[i];
        real_orientation *out = &node_orientations[i];
        out->rotation.n[0]     = node->default_rotation.v.n[0];
        out->rotation.n[1]     = node->default_rotation.v.n[1];
        out->rotation.n[2]     = node->default_rotation.v.n[2];
        out->rotation.n[3]     = node->default_rotation.w;   /* scalar (w) */
        out->translation.n[0]  = node->default_translation.n[0];
        out->translation.n[1]  = node->default_translation.n[1];
        out->translation.n[2]  = node->default_translation.n[2];
        out->scale             = 1.0f;
    }
}

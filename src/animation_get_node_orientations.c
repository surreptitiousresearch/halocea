/* animation_get_node_orientations @0x83796B48 — decode every node's local pose (rotation, translation, scale) for
 * one animation at one frame into a real_orientation array. If the animation is unusable for this model (overlay
 * type, node-list checksum mismatch, or node-count mismatch) it falls back to the model's bind pose.
 *
 * Each node has three independent channels, each gated by a per-node bit in one of the animation's flag arrays:
 *   bit set  -> channel is animated; value comes from the per-frame stream (or a keyframe helper when compressed)
 *   bit clear-> channel is default; value comes from the default-pose block (or, when compressed, a sub-table /
 *               an identity value).
 *
 * Two stream encodings are handled. Uncompressed: rotation is 4 packed int16 quaternion components scaled by
 * 1/32768, translation is 3 raw floats, scale is 1 raw float — each read advances a cursor. Compressed: animated
 * channels are fetched by per-channel keyframe ordinal via the animation_get_keyframe_* helpers; default rotation
 * is a 6-byte compressed quaternion indexed by node, default translation is indexed into a sub-table, and default
 * scale is identity. Reconstructed from disassembly — the decompiler's local allocation failed for this routine. */

#include <stdint.h>
#include "headers/model.h"
#include "headers/animation.h"
#include "headers/animation_flags.h"
#include "headers/real_orientation.h"
#include "headers/real_quaternion.h"
#include "headers/real_point3d.h"
#include "headers/compressed_quaternion_6byte.h"
#include "headers/animation_type.h"
#include "headers/blam_data_globals.h"

/* 0x38000100: int16 component -> normalized quaternion component (~1/32768). */
#define ROTATION_COMPONENT_SCALE 0.000030518509f


#include "headers/real_point3d.h"
#include "headers/real_quaternion.h"
extern void *animation_get_frame_data(const animation *animation, int16_t frame_index);
extern void animation_get_keyframe_rotation(const animation *animation, float real_frame_index, int16_t adjusted_node_index, int16_t node_index, real_quaternion *result);
extern void animation_get_keyframe_translation(const animation *animation, float real_frame_index, int16_t adjusted_node_index, int16_t node_index, real_point3d *result);
/* DEVIATION: disasm shows the scale helper receives the result pointer in r6 (4 args), not r7 — the DB prototype's
 * separate node_index param is not passed at this call site, so the extern is declared with 4 parameters. */
extern void animation_get_keyframe_scale(const animation *animation, float real_frame_index, int16_t adjusted_node_index, float *result);
extern void quaternion_decompress_6byte(const compressed_quaternion_6byte *compressed, real_quaternion *decompressed);
extern void quaternion_normalize(real_quaternion *q);
extern void model_get_node_orientations(const model *model, real_orientation *node_orientations);

extern int animation_is_compressed(const animation *animation);
void animation_get_node_orientations(const model *model, const animation *animation, int16_t frame_index, real_orientation *node_orientations)
{
    int use_animation_data = 1;
    if ( animation->type != animation_base )
    {
        use_animation_data = 0;
    }
    else if ( model )
    {
        unsigned int animation_checksum = animation->node_list_checksum;
        if ( animation_checksum != 0 && model->node_list_checksum != animation_checksum && model->node_list_checksum != 0 )
            use_animation_data = 0;
        else if ( model->nodes.count != animation->node_count )
            use_animation_data = 0;
    }

    if ( !use_animation_data )
    {
        model_get_node_orientations(model, node_orientations);
        return;
    }

    /* DEVIATION: collapsed inlined copy of animation_is_compressed@0x837956E0 (zero-xref donor; sole arg is the local `animation` pointer, no folding needed). */
    int compressed = animation_is_compressed(animation);

    char *frame_cursor = animation_get_frame_data(animation, frame_index);
    char *default_cursor = (char *)animation->default_data.address;

    short rotation_keyframe_ordinal = 0;
    short translation_keyframe_ordinal = 0;
    short scale_keyframe_ordinal = 0;

    unsigned int rotation_flags = 0;
    unsigned int translation_flags = 0;
    unsigned int scale_flags = 0;

    for ( short node_index = 0; node_index < animation->node_count; node_index = (short)(node_index + 1) )
    {
        real_orientation *node = &node_orientations[node_index];

        if ( (node_index & 0x1F) == 0 )
        {
            int word = node_index >> 5;
            translation_flags = animation->nodes_with_translation_flags[word];
            rotation_flags    = animation->nodes_with_rotation_flags[word];
            scale_flags       = animation->nodes_with_scale_flags[word];
        }

        /* ---- rotation ---- */
        if ( rotation_flags & 1 )
        {
            if ( compressed )
            {
                animation_get_keyframe_rotation(animation, (float)frame_index,
                    rotation_keyframe_ordinal, node_index, &node->rotation);
                rotation_keyframe_ordinal = (short)(rotation_keyframe_ordinal + 1);
            }
            else
            {
                const short *components = (const short *)frame_cursor;
                node->rotation.v.n[0] = (float)components[0] * ROTATION_COMPONENT_SCALE;
                node->rotation.v.n[1] = (float)components[1] * ROTATION_COMPONENT_SCALE;
                node->rotation.v.n[2] = (float)components[2] * ROTATION_COMPONENT_SCALE;
                node->rotation.w = (float)components[3] * ROTATION_COMPONENT_SCALE;
                frame_cursor += 8;
            }
        }
        else if ( compressed )
        {
            /* recovered: (compressed_quaternion_6byte *)(cursor + 6*node_index + cursor[1]) -> typed [] index off sub-table base */
            const compressed_quaternion_6byte *packed =
                &((const compressed_quaternion_6byte *)(frame_cursor + ((const int *)frame_cursor)[1]))[node_index];
            quaternion_decompress_6byte(packed, &node->rotation);
            quaternion_normalize(&node->rotation);
        }
        else
        {
            const short *components = (const short *)default_cursor;
            node->rotation.v.n[0] = (float)components[0] * ROTATION_COMPONENT_SCALE;
            node->rotation.v.n[1] = (float)components[1] * ROTATION_COMPONENT_SCALE;
            node->rotation.v.n[2] = (float)components[2] * ROTATION_COMPONENT_SCALE;
            node->rotation.w = (float)components[3] * ROTATION_COMPONENT_SCALE;
            default_cursor += 8;
        }
        rotation_flags >>= 1;

        /* ---- translation ---- */
        if ( translation_flags & 1 )
        {
            if ( compressed )
            {
                animation_get_keyframe_translation(animation, (float)frame_index,
                    translation_keyframe_ordinal, node_index, &node->translation);
                translation_keyframe_ordinal = (short)(translation_keyframe_ordinal + 1);
            }
            else
            {
                const float *components = (const float *)frame_cursor;
                node->translation.n[0] = components[0];
                node->translation.n[1] = components[1];
                node->translation.n[2] = components[2];
                frame_cursor += 12;
            }
        }
        else if ( compressed )
        {
            /* irreducible: compressed-animation blob cursor; [5] is the default-translation
             * table offset from the variable-format compressed header, stride 12 per node */
            const float *components =
                (const float *)(frame_cursor + 12 * node_index + ((const int *)frame_cursor)[5]);
            node->translation.n[0] = components[0];
            node->translation.n[1] = components[1];
            node->translation.n[2] = components[2];
        }
        else
        {
            const float *components = (const float *)default_cursor;
            node->translation.n[0] = components[0];
            node->translation.n[1] = components[1];
            node->translation.n[2] = components[2];
            default_cursor += 12;
        }
        translation_flags >>= 1;

        /* ---- scale ---- */
        if ( scale_flags & 1 )
        {
            if ( compressed )
            {
                animation_get_keyframe_scale(animation, (float)frame_index,
                    scale_keyframe_ordinal, &node->scale);
                scale_keyframe_ordinal = (short)(scale_keyframe_ordinal + 1);
            }
            else
            {
                node->scale = *(const float *)frame_cursor;
                frame_cursor += 4;
            }
        }
        else if ( compressed )
        {
            node->scale = 1.0f;
        }
        else
        {
            node->scale = *(const float *)default_cursor;
            default_cursor += 4;
        }
        scale_flags >>= 1;
    }
}

/* rasterizer_model_setup_model_shader @0x8378B588 — select the model pixel shader and technique for a
 * shader_model tag and bind it. Computes a technique-row index from the shader's detail_function plus flag- and
 * detail-mask-driven offsets (+3 when flag 0x1 is set; when flag 0x10 is clear, +12 if any detail mask is present
 * else +6). The detail_mask (0..8) then selects which of the six model shaders (117..121, resolved once by
 * rasterizer_model_init_shaders) and which technique table to use; odd/even mask pairs share a shader and 24-entry
 * table, the odd mask indexing at +6 and the even at +0. The chosen technique handle table[technique_row] is set
 * on the shader's effect. Returns the bound shader, or nullptr on any failure (mask out of range, shader/effect
 * missing, or set-technique error).
 *
 * DEVIATION: the decompiler's eight unk_8446Cxxx blobs are all offsets into the named technique tables declared
 * in rasterizer_model_techniques.h (verified against rasterizer_model_init_shaders: Reflection@C348,
 * SelfIllumination@C2B8, ChangeColor@C3A8, Multipurpose, NoMask@C318 — the +6 variants are &table[6]). The
 * redundant `&& detail_mask` guards Hex-Rays kept on the mask==3..7 arms are dead (each implies a nonzero mask)
 * and folded out; the per-branch null checks are hoisted after the switch since every arm performs them. */

#include "headers/rasterizer_model_techniques.h"
#include "headers/shader_model.h"
#include "headers/shader_model_flags.h"
#include "headers/rasterizer_dx9_shader_index.h"
#include "headers/shader_model_detail_mask.h"

extern long rasterizer_set_technique(ID3DXEffect *effect, unsigned int technique);

rasterizer_dx9_shader * rasterizer_model_setup_model_shader(shader_model *shader_model)
{
    unsigned __int16 flags = shader_model->model.flags;

    int technique_row = shader_model->model.detail_function;
    if ( (flags & (1u << _shader_model_detail_after_reflection_bit)) != 0 )
        technique_row += 3;
    if ( (flags & (1u << _shader_model_true_atmospheric_fog_bit)) == 0 )
        technique_row += shader_model->model.detail_mask ? 12 : 6;

    unsigned int detail_mask = (unsigned __int16)shader_model->model.detail_mask;
    if ( detail_mask >= NUMBER_OF_SHADER_MODEL_DETAIL_MASKS )
        return nullptr;

    rasterizer_dx9_shader *shader;
    unsigned int *techniques;
    switch ( detail_mask )
    {
        case _shader_model_detail_mask_reflection_mask_inverse:  shader = rasterizer_shader_select(_dxshader_model_reflection); techniques = &hModelReflectionTechniques[6];       break;
        case _shader_model_detail_mask_reflection_mask:  shader = rasterizer_shader_select(_dxshader_model_reflection); techniques = &hModelReflectionTechniques[0];       break;
        case _shader_model_detail_mask_self_illumination_mask_inverse:  shader = rasterizer_shader_select(_dxshader_model_self_illumination); techniques = &hModelSelfIlluminationTechniques[6]; break;
        case _shader_model_detail_mask_self_illumination_mask:  shader = rasterizer_shader_select(_dxshader_model_self_illumination); techniques = &hModelSelfIlluminationTechniques[0]; break;
        case _shader_model_detail_mask_change_color_mask_inverse:  shader = rasterizer_shader_select(_dxshader_model_change_color); techniques = &hModelChangeColorTechniques[6];      break;
        case _shader_model_detail_mask_change_color_mask:  shader = rasterizer_shader_select(_dxshader_model_change_color); techniques = &hModelChangeColorTechniques[0];      break;
        case _shader_model_detail_mask_multipurpose_alpha_mask_inverse:  shader = rasterizer_shader_select(_dxshader_model_multipurpose); techniques = &hModelMultipurposeMaskTechniques[6]; break;
        case _shader_model_detail_mask_multipurpose_alpha_mask:  shader = rasterizer_shader_select(_dxshader_model_multipurpose); techniques = &hModelMultipurposeMaskTechniques[0]; break;
        default: shader = rasterizer_shader_select(_dxshader_model_normal); techniques = &hModelNoMaskTechniques[0];           break;
    }

    if ( !shader || !shader->effect )
        return nullptr;

    if ( rasterizer_set_technique(shader->effect, techniques[technique_row]) < 0 )
        return nullptr;
    return shader;
}

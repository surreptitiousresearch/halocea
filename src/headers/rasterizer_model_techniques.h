#pragma once

#include "rasterizer_shader_helpers.h"

/* Per-mask model technique handle tables. The "NoMask" tables hold 12 entries
   (mask name indices 0..5 and 12..17); the others hold one per mask name (24).
   Names invented for the DB's unnamed globals (addresses noted). */
#ifdef __cplusplus
extern "C" {
#endif

extern char mask_technique_names[24][128];

extern unsigned int hModelEnvironmentNoMaskTechniques[12];     /* @0x8446C1D4.. */

#ifdef __cplusplus
}
#endif
extern unsigned int hModelSelfIlluminationTechniques[24];      /* @0x8446C2B8 */
extern unsigned int hModelChangeColorTechniques[24];           /* @0x8446C3A8 */
#ifdef __cplusplus
extern "C" {
#endif

extern unsigned int hModelMultipurposeMaskTechniques[24];

#ifdef __cplusplus
}
#endif
extern unsigned int hModelReflectionTechniques[24];            /* @0x8446C348 */
extern unsigned int hModelNoMaskTechniques[12];                /* @0x8446C318 */

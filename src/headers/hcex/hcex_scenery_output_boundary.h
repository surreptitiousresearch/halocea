#pragma once
/* Boundary declarations for hcex_scenery_output.c.
 *
 * hcex_scenery_output is an HCEX debug helper: it appends one placed object's model name, halo->ws
 * converted position, and euler orientation to the "scenery_obj_list.txt" dump log. Everything it calls is a
 * ws-engine boundary (the m3d math library and the apLog dump-log system) plus the hcex model-name
 * sanitiser; none are reversed here. m3dV/m3dMATR come from hcex_prepare_locations_boundary.h. */

#include "hcex_prepare_locations_boundary.h"   /* m3dV, m3dMATR, hcex_float3 */

/* halo world unit -> ws unit scale (1 world unit = 3.0480001). */
#define HALO_TO_WS_SCALE 3.0480001f

extern const m3dV m3dVZero;

/* --- ws m3d math library (boundary) --- */
extern void m3dNormalize(m3dV *v);
extern void m3dCrossProduct(const m3dV *a, const m3dV *b, m3dV *out);
extern void m3dMATR_MakeLCS2WCS(m3dMATR *out, const m3dV *origin,
        const m3dV *forward, const m3dV *up, const m3dV *third);
extern void m3dMATR_GetRotateRot(const m3dMATR *m, float *rot0, float *rot1, float *rot2);

/* --- ws apLog dump-log system (boundary) --- */
extern void apLogSetDumpRightNowMode(int enable);
extern void _apDumpLog(const char *file, const char *fmt, ...);

/* --- hcex model-name sanitiser --- */
extern "C" char *hcex_conv_mdl_name(const char *model_name, char *out, int max_len);

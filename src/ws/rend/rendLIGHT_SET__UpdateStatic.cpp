#include <string.h>
#include "../../headers/ws/rend/rendLIGHT_SET.h"
#include "../../headers/ws/rend/rendLGT_MNG.h"
#include "../../headers/ws/rend/rendLIGHT.h"
#include "../../headers/ws/rend/STATIC_LIGHT_ENTRY.h"
#include "../../headers/ws/rend/DEBUG_LIGHT_DATA.h"
#include "../../headers/ws/cdt/cdtCOLL_LIGHT.h"
#include "../../headers/ws/ds/dsCONST_ARRAY.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/anim/animINST.h"
#include "../../headers/ws/m3d/m3d_boundary.h"
#include "../../headers/ws/wb/dbgVAR_boundary.h"

// rendLIGHT_SET::UpdateStatic @ 0x82ABDE20
//
// Recompute the baked static lighting sampled at world position `pos`, writing the result into
// `res` (its SH color plus the shader-directional-response ambient term). The routine:
//   1. gathers a candidate list of static lights: every "big" light, every light bucketed into
//      the voxel-grid cell containing `pos`, and the special "neg_tunnel_0" lights;
//   2. evaluates each candidate's attenuation and lamp visibility at `pos`;
//   3. optionally records the player's lights for the debug overlay;
//   4. accumulates the surviving lights into res->shColor and res->sdrCoeffs.ambient, treating
//      pure-ambient (type 2), "neg_"-prefixed and "pos_"-prefixed lights specially.
//
// Transcription notes (this is one of the batch's large bodies):
//  * The decompiler heavily register-puns 64-bit locals; those are resolved here to the typed
//    accesses the DB layouts imply (lgtLIGHT::colorD/type/name/matrL2W, rendLIGHT::lightData).
//  * dsCONST_ARRAY::operator[] inlines an IsValidIdx STRONG_ASSERT at every access; those repeated
//    bounds-check crashes are elided here for readability (single deviation, behavior-preserving)
//    and expressed as plain candidateList.list[idx] indexing.
//  * The manual UnsafeInit + char-compare the decompiler emits for the "neg_"/"pos_" prefix tests
//    is a temporary dsTSTRING view over the light's name; reduced here to the observable strncmp
//    against the light's name buffer.

// --- boundary globals / helpers (rend + debug subsystems) ---
extern "C" dbgVAR_SIMPLE<bool,  1> dbg_isOffRenderAnim;            // boundary (only .value read)
extern "C" dbgVAR_SIMPLE<bool,  1> dbg_lightSetShowAllLights;      // boundary
extern "C" dbgVAR_SIMPLE<float, 3> dbg_lightSetAmbFilterThreshold; // boundary
extern dsVECTOR<DEBUG_LIGHT_DATA, 8> dbgPlayerLights;          // boundary — debug overlay list

static const float kAttenEpsilon = 0.000001f;

void rendLIGHT_SET::UpdateStatic(rendSTATIC_LIGHTING_STATE *res, m3dV *pos)
{
    memset(res, 0, sizeof(rendSTATIC_LIGHTING_STATE));

    // Candidate list, cleared. (The prologue zero-fill loop initialises each entry's light ptr
    // and attenuation.)
    dsCONST_ARRAY<STATIC_LIGHT_ENTRY, 2048> candidateList;
    candidateList.nEntry = 0;
    for (int i = 0; i < 2048; ++i) {
        candidateList.list[i].light = nullptr;
        candidateList.list[i].atten = 0.0f;
    }

    // 1a. Every "big" light.
    for (int i = 0; i < rendLgtMng->lastBigLightNmb; ++i) {
        const rendLIGHT_SP *sp = rendLgtMng->GetStaticLight(i);
        if (sp->pointee) {
            STATIC_LIGHT_ENTRY entry;
            entry.light = sp->pointee->lightData;
            entry.atten = 0.0f;
            candidateList.Add(entry);
            candidateList.Back();
        }
    }

    // 1b. Lights bucketed into the voxel-grid cell containing `pos`.
    cdtCOLL_LIGHT *pCollLight = rendLgtMng->pCollLight;
    if (pCollLight) {
        int ix, iy, iz;
        pCollLight->voxGrid.GetPosInd(pos, &ix, &iy, &iz, 1);

        cdtCOLL_LIGHT *coll = rendLgtMng->pCollLight;
        int cell = (iy + coll->voxGrid.nVoxel.y * ix) * coll->voxGrid.nVoxel.z + iz;

        // start/end offsets into idList (the top bit of each start entry is a flag — masked off).
        unsigned int cellEnd   = coll->startIDList[cell + 1] & 0x7FFFFFFF;
        unsigned int cellStart = coll->startIDList[cell]     & 0x7FFFFFFF;
        int count = (int)(cellEnd - cellStart);

        for (int j = 0; j < count; ++j) {
            unsigned int base = rendLgtMng->pCollLight->startIDList[cell] & 0x7FFFFFFF;
            unsigned short lightId = rendLgtMng->pCollLight->idList[(int)(base + j)];
            const rendLIGHT_SP *sp = rendLgtMng->GetStaticLight(lightId);
            if (sp->pointee) {
                STATIC_LIGHT_ENTRY entry;
                entry.light = sp->pointee->lightData;
                entry.atten = 0.0f;
                candidateList.Add(entry);
            }
        }
    }

    // 1c. The special "neg_tunnel_0" lights (added only when they actually reach `pos`).
    for (int i = 0; i < rendLgtMng->staticLights.nElem; ++i) {
        const rendLIGHT_SP *sp = rendLgtMng->GetStaticLight(i);
        rendLIGHT *pointee = sp->pointee;
        if (strstr(pointee->lightData->name.pBuffer->str, "neg_tunnel_0")) {
            if (pointee->lightData->CalcAttenuation(pos) > kAttenEpsilon) {
                STATIC_LIGHT_ENTRY entry;
                entry.light = pointee->lightData;
                entry.atten = 0.0f;
                candidateList.Add(entry);
            }
        }
    }

    int nEntry = candidateList.nEntry;

    // 2. Evaluate per-light attenuation + lamp visibility at `pos`.
    srData.StartLampsUpdate(pos);
    for (int i = 0; i < nEntry; ++i) {
        lgtLIGHT *light = candidateList.list[i].light;
        float atten = light->CalcAttenuation(pos);
        if (atten > kAttenEpsilon && (light->state.val & 1) != 0 &&
            !srData.IsLampVisible(light->id)) { // takes const lgtID&
            atten = 0.0f;
        }
        candidateList.list[i].atten = atten;
    }
    srData.FinishLampsUpdate();

    // 3. Debug overlay: record the player's enabled lights.
    if (dbg_lightSetShowAllLights.value) {
        animINST *inst = pInstOwner;
        if (inst) {
            const char *name = inst->name;
            if (!name)
                name = ""; // decompiler uses the shared empty-string empty_string
            if (!stricmp(name, "player")) {
                dbgPlayerLights.nElem = 0;
                for (int i = 0; i < nEntry; ++i) {
                    float atten = candidateList.list[i].atten;
                    if (atten >= kAttenEpsilon) {
                        lgtLIGHT *light = candidateList.list[i].light;
                        if ((light->state.val & 1) != 0) {
                            DEBUG_LIGHT_DATA d;
                            d.atten = atten;
                            d.pos.x = light->matrL2W.elements[12];
                            d.pos.y = light->matrL2W.elements[13];
                            d.pos.z = light->matrL2W.elements[14];
                            dbgPlayerLights.PushBack(d); // takes const T&
                        }
                    }
                }
            }
        }
    }

    // 4. Accumulate contributions into the SH color and the SDR ambient term.
    float maxNeg = 0.0f;    // strongest "neg_" light (subtracted from ambient alpha at the end)
    float maxPos = 0.0f;    // strongest "pos_" light (added back)
    int   hasLight = 0;
    m3dCOLOR ambColor = { 1.0f, 1.0f, 1.0f, 1.0f }; // scratch color for AddLightAmb

    if (nEntry) {
        for (int i = 0; i < nEntry; ++i) {
            float atten = candidateList.list[i].atten;
            if (atten < kAttenEpsilon)
                continue;

            hasLight = 1;
            lgtLIGHT *light = candidateList.list[i].light;

            // Per-light color scaled by attenuation.
            m3dCOLOR weighted;
            weighted.r = light->colorD.r * atten;
            weighted.g = light->colorD.g * atten;
            weighted.b = light->colorD.b * atten;
            weighted.a = light->colorD.a * atten;

            if (light->type == 2) {
                // Pure-ambient light: add unweighted color into shColor + SDR ambient.
                res->shColor.AddLightAmb(1.0f, &ambColor);
                res->sdrCoeffs.ambient.r += light->colorD.r;
                res->sdrCoeffs.ambient.g += light->colorD.g;
                res->sdrCoeffs.ambient.b += light->colorD.b;
                res->sdrCoeffs.ambient.a += light->colorD.a;
                continue;
            }

            const char *lname = light->name.pBuffer->str;
            if (!strncmp(lname, "neg_", 4)) {
                float c = m3dClamp(0.0f, 1.0f, 0.0f, 1.0f, atten);
                if (maxNeg < c)
                    maxNeg = c;
            } else if (!strncmp(lname, "pos_", 4)) {
                float c = m3dClamp(0.0f, 1.0f, 0.0f, 1.0f, atten);
                if (maxPos < c)
                    maxPos = c;
            } else {
                // Directional light: add to the SH color along its world-space direction.
                m3dV dir;
                dir.x = light->matrL2W.elements[12] - pos->x;
                dir.y = light->matrL2W.elements[13] - pos->y;
                dir.z = light->matrL2W.elements[14] - pos->z;
                _m3dNormalize(&dir);
                res->shColor.AddLight(&dir, &weighted);

                // Below a luminance threshold the light also feeds the flat SDR ambient.
                float luminance = weighted.g * 0.58999997f
                                + (weighted.b * 0.11f + weighted.r * 0.30000001f);
                if (luminance < dbg_lightSetAmbFilterThreshold.value) {
                    res->sdrCoeffs.ambient.r += weighted.r;
                    res->sdrCoeffs.ambient.g += weighted.g;
                    res->sdrCoeffs.ambient.b += weighted.b;
                    res->sdrCoeffs.ambient.a += weighted.a;
                }
            }
        }
    }

    if (!hasLight) {
        // No contributing light: seed a flat 0.7 ambient.
        ambColor.r = ambColor.g = ambColor.b = ambColor.a = 1.0f;
        res->shColor.AddLightAmb(0.69999999f, &ambColor);
    }

    res->shColor.CalcShaderConstants(&res->sdrCoeffs.shCoeffs);
    res->sdrCoeffs.ambient.a = m3dClamp(0.0f, 1.0f, 0.0f, 1.0f, maxNeg - maxPos);
}

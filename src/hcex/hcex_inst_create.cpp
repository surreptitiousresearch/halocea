/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* hcex_inst_create @0x823E74B8 — the HCEX bridge's core "spawn a ws-engine instance for a Blam
 * object/model" entry point. Given either a Halo model tag (modelId != -1) or a plain object id
 * (id), resolves (or registers, on first use) an HCEX_MODEL describing the model's regions/nodes,
 * resolves the ws-engine template to instantiate, creates the animINST (through an owning entity
 * when the resolved brand carries one, otherwise directly via the scene), tags every region/object
 * name against the live instance's node tree (first registration only), and — for first-person
 * models (isFP) — installs an HCEX_ANIM_MNG_FP as the instance's animation manager.
 *
 * DEVIATIONS from the raw decompiler output (verified against the disassembly at each call site):
 *  - scnSCENE::CreateInst(animTPL*, animCREATE_DATA*) takes exactly those two arguments; the extra
 *    v66..v62 the decompiler shows are uninitialised locals it mis-attributed to the call.
 *  - Several inlined "does this string equal X" loops (comparing v9/hcex conv-name buffers against
 *    literal object-class names) are byte-for-byte equivalent to strcmp(...)==0 and are written that
 *    way here for readability; they are not calls to the real strcmp in the binary (the compiler
 *    inlined the compare), so no behavior changes.
 *  - hcexModels/HCEX_MODEL_REGION/objNames element access is done via `.pData[i]` rather than
 *    `operator[]` — those specific dsVECTOR<T,8> instantiations' operator[] have not been reversed
 *    as their own subroutines in this batch (their bodies are all the same trivial `pData[i]`;
 *    reproduced here directly rather than declaring N one-line boundary thunks).
 *  - The final region-name/permutation/object-name resolution loop is reproduced structurally but
 *    the objFILTER_NAME_NOCASE construction is written via its real constructor (the decompiler
 *    shows raw vtable+name-pointer field stores, which is exactly what that constructor does).
 *
 * spDesc bit-20 gate (`extrwi r8,r9,1,20`): now typed via iaDESC::state @0x70 (DB) — it gates a
 * single state-bit clear (`pInst->state &= ~2`) for entity-backed instances; see the in-body note. */

#include "../headers/hcex/HCEX_MODEL.h"
#include "../headers/hcex/HCEX_ANIM_MNG_FP.h"
#include "../headers/hcex/hcex_matr4x3.h"
#include "../headers/ws/anim/animCREATE_DATA.h"
#include "../headers/ws/anim/animTPL.h"
#include "../headers/ws/ent/entENTITY.h"
#include "../headers/ws/ia/iaDESC.h"
#include "../headers/ws/ent/entDESC.h"
#include "../headers/ws/obj/objOBJ.h"
#include "../headers/ws/obj/objFILTER_NAME_NOCASE.h"
#include "../headers/ws/obj/obj_free_functions.h"
#include "../headers/ws/scn/scnSCENE.h"
#include "../headers/ws/gs/gsLVL_SYSTEM.h"
#include "../headers/ws/dsc/dscDESC_FAMILY.h"
#include "../headers/ws/ds/dsPAIR.h"
#include "../headers/ws/ds/dsCMP.h"
#include "../headers/ws/ds/ds_assert_boundary.h"
#include <string.h>
#include <stdio.h>
#include <new>

/* --- HCEX bridge helpers already reversed elsewhere in src/hcex --- */
extern "C" char *hcex_model_name(int modelId);                                   /* tag path for a model tag */
extern "C" char *hcex_conv_mdl_name(const char *mdl_name, char *out, int max_len);
extern "C" char *hcex_obj_definition_name(int object_index);
extern "C" const char *hcex_obj_prefix(int id);
extern "C" char *hcex_conv_cls_name(const char *name, const char *name_prefix, char *out, int max_len);
extern "C" char *hcex_conv_name(char *name, int max_len);
extern "C" int   hcex_obj_scenery_idx_type(int id, int *obj_idx, int *obj_type);
extern "C" int   hcex_get_obj_color(int id);
extern "C" int   hcex_get_obj_matr(int objId, hcex_matr4x3 *obj_matr);
extern void  hcex_make_inst_matr(const hcex_matr4x3 *in, m3dMATR *out);
extern "C" void  hcex_enum_model_nodes(int model_index, void (*cb)(void *mdl, const char *node), void *mdl);
extern "C" void  hcex_enum_model_regions(int model_index, void (*cb)(void *mdl, const char *region), void *mdl);
extern "C" void  hcex_enum_model_perm(int model_index, int region_index,
                 void (*cb)(void *mdl, int region_index, const char *permutation), void *mdl);
extern "C" void  hcex_add_model_node(void *mdl, const char *node_name);
extern void  hcex_add_model_reg(void *mdl, const char *reg_name);
extern void  hcex_add_model_perm(void *mdl, int idx, const char *perm_name);

/* --- Blam-side lookups --- */
extern "C" int player_index_from_unit_index(int unit_index);
extern "C" int  hcex_spawning_player;

/* --- globals this function reads/writes --- */
extern scnSCENE                                      *gsScenePtr;
extern gsLVL_SYSTEM                                  *gsSysLevel;
extern dscDESC_FAMILY                                 iaFamily;
extern dsVECTOR<dsPAIR<unsigned long, HCEX_MODEL>, 8>  hcexModels;
extern dsVECTOR<dsTSTRING<char>, 8>                    hcexClsList;   /* debug: used-class dump */
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

/* HCEX-specific custom-object registry: a large, separately-gated bridge class (not reversed in
 * this batch). Only the one method hcex_inst_create calls is declared. */
struct hcexHALO_CUSTOM_OBJ_LIST {
    // 0x8253D5D8 — resolve the ws class/template names to use for a Blam scenery object at
    // (obj_idx, obj_type): `outClsName`/`outHaloName` receive the resolved ws class name and Halo
    // tag-derived name, `outTplName` the resolved template name override (if any). Returns whether
    // a custom registration was found. boundary — declared only for this call site.
    bool GetClsTplName(int obj_idx, int obj_type, dsTSTRING<char> &outClsName,
                        dsTSTRING<char> &outHaloName, dsTSTRING<char> &outTplName);
};
struct instMANAGER_HANDLER; // boundary — base handler type instManager is declared as
extern instMANAGER_HANDLER *instManager; /* really an hcexHALO_CUSTOM_OBJ_LIST (polymorphic_downcast) */

/* farmSYSTEM's isSuspendAdd flag, at its DB-verified offset (268 = 0x10C). Restated locally with
 * correct padding rather than reusing farm_system.h's simplified (offset-0) stand-in, since this
 * function's disassembly reads/writes the flag at +0x10C directly. */
struct farmSYSTEM_bridge { unsigned char _pad[0x10C]; bool isSuspendAdd; };
extern farmSYSTEM_bridge *farmSys;

extern animTPL *gsExtFindTplName(const char *name, void *affixes, int flags);
extern entENTITY *entCreate(scnSCENE *scene, const char *nameClass, const char *nameTpl,
                struct entCREATE_DATA *pCD, void *pMsgData);

#include "../headers/entCREATE_DATA.h"

extern "C" int sprintf_0(char *string, const char *format, ...);
void _apLog(const char *format, ...);
extern "C" void dlFree(void *ptr);

/* dbgVAR_bool (DB-verified, 16 bytes: base dbgVAR@0 + value/prevValue/defaultValue @0xC/D/E). */
struct dbgVAR_bool_min { unsigned char _base[12]; unsigned char value; };
extern "C" dbgVAR_bool_min dbg_hcexDumpUsedClsOn;

extern dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);

extern void *operator new(size_t size, const char *file, unsigned int line); // boundary — debug-tagged new (Saber)

animINST *hcex_inst_create(int modelId, int id, bool isFP, int *pModelIdx)
{
    animCREATE_DATA createData;
    sprintf_0(createData.name, "id%x", id);

    if (!gsScenePtr)
        return 0;

    char hcexName[64];
    memset(hcexName, 0, sizeof(hcexName));

    const char *haloName = 0;
    if (modelId != -1) {
        haloName = hcex_model_name(modelId);
        if (haloName)
            hcex_conv_mdl_name(haloName, hcexName, 64);
    }

    const char *clsName;
    char clsNameBuf[224]; /* backs clsName's default (non-special-cased) value below */
    {
        const char *definitionName = isFP ? 0 : hcex_obj_definition_name(id);
        const char *prefix = hcex_obj_prefix(id);
        clsName = hcex_conv_cls_name(definitionName, prefix, clsNameBuf, 64);

        if (haloName) {
            if (strcmp(haloName, "characters\\cyborg\\cyborg") == 0
                && (hcex_spawning_player != -1 || player_index_from_unit_index(id) != -1))
                clsName = "halo_player";

            size_t len = strlen(hcexName);
            if (len >= 3 && strcmp(hcexName + len - 3, "_fp") == 0)
                clsName = hcexName;
        }
    }

    dsTSTRING<char> haloNameStr; haloNameStr.pBuffer = 0; /* filled below once resolved */
    haloNameStr.UnsafeInitEmpty();

    if (!isFP) {
        int objIdx = 0, objType = 0;
        if (hcex_obj_scenery_idx_type(id, &objIdx, &objType) && objIdx > 0 && objType >= 0) {
            dsTSTRING<char> outCls; outCls.pBuffer = 0; outCls.UnsafeInitEmpty();
            dsTSTRING<char> outHalo; outHalo.pBuffer = 0; outHalo.UnsafeInitEmpty();
            dsTSTRING<char> outTpl; outTpl.pBuffer = 0; outTpl.UnsafeInitEmpty();

            bool found = ((hcexHALO_CUSTOM_OBJ_LIST *)instManager)
                             ->GetClsTplName(objIdx, objType, outCls, outHalo, outTpl);
            if (found) {
                strncpy(createData.name, outTpl.pBuffer->str, 0x7F);
                createData.name[126] = 0;
            }

            /* Special-case template overrides mirrored from the decompile (byte-for-byte
             * strcmp against the resolved class name). */
            if (strcmp(outCls.pBuffer->str, "halo_object") == 0) {
                dsTSTRING<char> defaultCls; defaultCls.pBuffer = 0;
                defaultCls.UnsafeInit(empty_string, -1, 0);
                outCls = defaultCls;
            }
            if (strcmp(outCls.pBuffer->str, "halo_no_model") == 0) {
                /* GetClsTplName found no usable registration for this scenery slot: bail out
                 * exactly like the "no template resolved" path below. */
                return 0;
            }

            clsName = outCls.pBuffer->str; /* outCls/outHalo/outTpl release at scope exit */
            haloNameStr = outHalo;
        }
    }

    if (haloNameStr.pBuffer->strLen)
        clsName = haloNameStr.pBuffer->str;

    if (dbg_hcexDumpUsedClsOn.value) {
        dsTSTRING<char> entry; entry.pBuffer = 0;
        dsSPrintf(&entry, "'%s' '%s'", clsName, haloName);
        hcexClsList.InsertSorted(entry, dsCMP(), dsVECTOR<dsTSTRING<char>, 8>::INS_DUP_IGNORE);
        if (entry.pBuffer->refCount-- == 1)
            dlFree(entry.pBuffer);
    }

    dscBRAND *brand = 0;
    if (clsName) {
        dsTSTRING<char> clsNameStr; clsNameStr.pBuffer = 0;
        clsNameStr.UnsafeInit(clsName, -1, 0);
        brand = iaFamily.FindBrand(clsNameStr);
        if (brand) {
            char *defaultTpl = ((entDESC *)brand->spDesc.pointee)->GetNameTplDefault();
            strncpy(hcexName, defaultTpl, 0x40);
            hcexName[63] = 0;
        }
    }

    if (haloNameStr.pBuffer->strLen) {
        strncpy(hcexName, haloNameStr.pBuffer->str, 0x40);
        hcexName[63] = 0;
    }

    animTPL *tpl = gsExtFindTplName(hcexName, 0, 0);
    if (!brand && !haloNameStr.pBuffer->strLen) {
        if (tpl) {
            createData.scaleX = 0.030479999f;
            createData.scaleY = 0.030479999f;
            createData.scaleZ = 0.030479999f;
        } else {
            strncat(hcexName, "__h", 0x40 - strlen(hcexName) - 1);
            tpl = gsExtFindTplName(hcexName, 0, 0);
            if (!tpl) {
                _apLog("~HCEX,Error~ Can't load tpl '%s'", hcexName);
                strcpy(hcexName, "unnamed"); /* algn_82003A02+1, a 9-byte fallback literal */
                tpl = gsExtFindTplName(hcexName, 0, 0);
            }
        }
    }

    if (!tpl) {
        _apLog("~HCEX,Error~ Can't load tpl '%s'", hcexName);
        return 0;
    }

    if (!IGNORE_STRONG_ASSERT && tpl->id > 0xFFFF)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("pTpl->id <= 65535",
            "D:\\Projects\\code\\HCEX\\sources\\halo_main.cpp", 716, empty_string);

    unsigned int modelKey = ((unsigned int)tpl->id << 16) | (unsigned short)modelId;

    int foundIdx = -1;
    for (int i = 0; i < hcexModels.nElem; ++i) {
        if (hcexModels.pData[i].key == modelKey) { foundIdx = i; break; }
    }

    if (foundIdx < 0) {
        HCEX_MODEL newModel;
        newModel.modelId = modelId;
        hcex_enum_model_nodes(modelId, hcex_add_model_node, &newModel);
        hcex_enum_model_regions(modelId, hcex_add_model_reg, &newModel);
        for (int r = 0; r < newModel.regions.nElem; ++r)
            hcex_enum_model_perm(modelId, r, hcex_add_model_perm, &newModel);

        dsTSTRING<char> haloNameTs; haloNameTs.pBuffer = 0;
        haloNameTs.UnsafeInit(haloName, -1, 0);
        newModel.haloName = haloNameTs;

        dsTSTRING<char> hcexNameTs; hcexNameTs.pBuffer = 0;
        hcexNameTs.UnsafeInit(hcexName, -1, 0);
        newModel.hcexName = hcexNameTs;

        dsPAIR<unsigned long, HCEX_MODEL> entry = { modelKey, newModel }; // val copy-constructed
        hcexModels.PushBack(entry);
    }

    if (!isFP) {
        hcex_matr4x3 objMatr;
        hcex_get_obj_matr(id, &objMatr);
        hcex_make_inst_matr(&objMatr, &createData.matrInst);
    }

    entENTITY *ent = 0;
    animINST  *pInst = 0;
    if (brand) {
        entCREATE_DATA entCreateData;
        (animCREATE_DATA &)entCreateData = createData;

        bool wasSuspended = farmSys->isSuspendAdd;
        farmSys->isSuspendAdd = true;
        ent = entCreate(gsScenePtr, clsName, hcexName, &entCreateData, 0);
        farmSys->isSuspendAdd = wasSuspended;

        if (!ent)
            return 0;
        pInst = ent->pInst;
    } else {
        pInst = gsScenePtr->CreateInst(tpl, &createData);
        gsSysLevel->InstLoadNotify(pInst);
    }

    if (!pInst)
        return 0;

    pInst->state |= 2;
    pInst->state2 |= 0x200200;

    /* typed (DB types_members iaDESC::state @0x70, apSTATE_T<unsigned long>): earlier note
     * misattributed this word to psCustomSection (@0x6C); it is the desc state flags, bit 20.
     * spDesc's static pointee type is dscDESC; the runtime desc of an actor is an iaDESC. */
    if (ent && ((((iaDESC *)ent->spDesc.pointee)->state.state >> 20) & 1))
        pInst->state &= ~2;

    gsSysLevel->InstLoadNotify(pInst);
    pInst->pObj->SetRenderPassId(2);

    if (foundIdx < 0) {
        HCEX_MODEL *model = &hcexModels.Back().val;

        if (model->regions.nElem == 1 && model->regions.pData[0].objNames.nElem <= 1)
            model->regions.Clear();

        for (int r = 0; r < model->regions.nElem; ++r) {
            HCEX_MODEL_REGION *region = &model->regions.pData[r];

            char regionName[64];
            strncpy(regionName, region->name.pBuffer->str, 0x40);
            regionName[63] = 0;
            hcex_conv_name(regionName, 64);

            objFILTER_NAME_NOCASE regionFilter(regionName);
            objOBJ *regionObj = objFind(pInst->pObj, &regionFilter);
            region->objRegId = regionObj ? (short)regionObj->id : -1;

            if (regionObj) {
                for (int n = 0; n < region->objNames.nElem; ++n) {
                    char objName[64];
                    strncpy(objName, region->objNames.pData[n].pBuffer->str, 0x40);
                    objName[63] = 0;
                    hcex_conv_name(objName, 64);

                    objFILTER_NAME_NOCASE objFilter(objName);
                    objOBJ *foundObj = objFind(regionObj, &objFilter);
                    short objId = foundObj ? (short)foundObj->id : -1;
                    region->objIds.PushBack(objId);

                    if (!foundObj)
                        _apLog("~HCEX,Error~ Can't find object '%s' in '%s' tpl for region '%s' of model '%s'",
                            objName, model->hcexName.pBuffer->str, regionName, model->haloName.pBuffer->str);
                }
            } else {
                _apLog("~HCEX,Error~ Can't find region object '%s' in '%s' tpl for '%s' model",
                    regionName, model->hcexName.pBuffer->str, model->haloName.pBuffer->str);
            }
        }

        if (pInst->nObj > model->nodeIndices.allocated)
            model->nodeIndices.Realloc(pInst->nObj);
        for (int n = 0; n < pInst->nObj; ++n) {
            short unresolved = -1;
            model->nodeIndices.PushBack(unresolved);
        }

        for (int n = 0; n < model->objNames.nElem; ++n) {
            char objName[64];
            strncpy(objName, model->objNames.pData[n].pBuffer->str, 0x40);
            objName[63] = 0;
            hcex_conv_name(objName, 64);

            objFILTER_NAME_NOCASE objFilter(objName);
            objOBJ *foundObj = objFind(pInst->pObj, &objFilter);
            if (foundObj)
                model->nodeIndices.pData[foundObj->id] = (short)n;
            else
                _apLog("~HCEX,Error~ Can't find object '%s' in '%s' tpl for '%s' model",
                    objName, model->hcexName.pBuffer->str, model->haloName.pBuffer->str);
        }

        foundIdx = hcexModels.nElem - 1;
    }

    if (pModelIdx)
        *pModelIdx = foundIdx;

    if (isFP) {
        HCEX_ANIM_MNG_FP *fpMng = (HCEX_ANIM_MNG_FP *)operator new(0xF18u,
            "D:\\Projects\\code\\HCEX\\sources\\halo_main.cpp", 0x356u);
        if (fpMng) {
            new (fpMng) HCEX_ANIM_MNG_FP(pInst);
            pInst->pAnimMng = fpMng;
            pInst->state |= 0x4400u;
        } else {
            pInst->pAnimMng = 0;
            pInst->state |= 0x4400u;
        }
    } else {
        const char *nameTpl = pInst->nameTpl ? pInst->nameTpl : empty_string;
        if (strcmp(nameTpl, "grunt") == 0) {
            unsigned int color = hcex_get_obj_color(id);
            if (color != 0xFFFFFFu)
                pInst->pObj->SetColorMPHier(color);
        }
    }

    if (!pInst->pEnt)
        pInst->state |= 0x1000u;
    pInst->state2 |= 4u;

    return pInst;
}

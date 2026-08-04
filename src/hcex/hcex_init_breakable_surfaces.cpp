/* hcex_init_breakable_surfaces @0x823B21F8 — recreate the ws-engine scene instance for every breakable-surface
 * slot from its persisted create-data: build an animCREATE_DATA descriptor (name, state, transform, particle
 * section, affixes, decomposed scale), resolve the template, create the instance, flag it, and — if the slot
 * carries an entity-class name — spawn the wrapping farm entity with add suspended.
 *
 * hcex-bridge code; the ws-engine anim/scene/level/entity/matrix primitives are declared as boundary externs.
 * Deviation 1: debug-only STRONG_ASSERT (slot must start with pInst==0) elided.
 * Deviation 2: the decompiler renders the particle-section copy as reading matrInst.elements[10]; the real
 * source is the create-data's own ps field (v10.ps = cd->ps) — corrected here. */

#include "../headers/hcex/hcex_breakable_surf.h"
#include "../headers/hcex/hcex_surf_vector.h"
#include "../headers/ws/anim/animCREATE_DATA.h" // full animCREATE_DATA (local descriptor by value)

extern "C" char *strncpy(char *dst, const char *src, size_t n); // boundary — CRT

extern hcex_surf_vector hcexBreakableSurfaces;
extern struct scnSCENE *gsScenePtr;
extern struct gsLEVEL *gsSysLevel;
#include "../headers/ws/farm/farmSYSTEM.h" /* farmSYSTEM (isSuspendAdd@0x10C) */
extern farmSYSTEM *farmSys;

extern void  animCREATE_DATA_ctor(animCREATE_DATA *cd);
extern void  animCREATE_DATA_dtor(animCREATE_DATA *cd);
extern void  psSECTION__assign(psSECTION *dst, const psSECTION *src);
extern void  dsAFFIX_STRING_assign(dsAFFIX_STRING *dst, const dsAFFIX_STRING *src);
extern void  m3dMATR_GetScale(m3dMATR *matr, float *sx, float *sy, float *sz);
extern void  m3dMATR_RemoveScale(m3dMATR *matr);
extern struct animTPL *gsLVL_SYSTEM_FindTplName(struct gsLEVEL *level, const char *name, dsAFFIX_STRING *affixes, int flags);
extern animINST *scnSCENE_CreateInst(struct scnSCENE *scene, struct animTPL *tpl, animCREATE_DATA *cd);
extern void  entCreate(const char *className, animINST *instance, void *parent);

void hcex_init_breakable_surfaces(void)
{
    for ( int i = 0; i < hcexBreakableSurfaces.nElem; ++i )
    {
        scnINST_CREATE_DATA *cd = &hcexBreakableSurfaces.pData[i].cd;

        animCREATE_DATA create_data;
        animCREATE_DATA_ctor(&create_data);

        strncpy(create_data.name, cd->nameInst.pBuffer->str, 0x7F);
        create_data.name[126] = 0;
        create_data.state = cd->state;
        create_data.matrInst = cd->matrInst;
        psSECTION__assign(&create_data.ps, &cd->ps);  /* deviation 2 */
        dsAFFIX_STRING_assign(&create_data.affixes, &cd->affixes);

        float scaleX, scaleY, scaleZ;
        m3dMATR_GetScale(&create_data.matrInst, &scaleX, &scaleY, &scaleZ);
        m3dMATR_RemoveScale(&create_data.matrInst);
        create_data.scaleX = scaleX;
        create_data.scaleY = scaleY;
        create_data.scaleZ = scaleZ;

        struct animTPL *tpl = gsLVL_SYSTEM_FindTplName(gsSysLevel, cd->nameTpl.pBuffer->str, &create_data.affixes, 0);
        if ( tpl )
        {
            animINST *instance = scnSCENE_CreateInst(gsScenePtr, tpl, &create_data);
            if ( instance )
            {
                hcexBreakableSurfaces.pData[i].pInst = instance;
                instance->state |= 2u;

                if ( cd->nameClass.pBuffer->strLen )
                {
                    unsigned char wasSuspended = farmSys->isSuspendAdd;
                    farmSys->isSuspendAdd = 1;
                    entCreate(cd->nameClass.pBuffer->str, instance, 0);
                    farmSys->isSuspendAdd = wasSuspended;
                }
            }
        }

        animCREATE_DATA_dtor(&create_data);
    }
}

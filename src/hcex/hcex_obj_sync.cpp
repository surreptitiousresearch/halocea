/* hcex_obj_sync @0x823DCEE8 — push one tracked HCEX_OBJ's full per-frame render state into its
 * ws-engine animINST:
 *   1. camera-visibility latches, derived from the Blam "hidden from camera" mask
 *      (hcex_object_hidden_cam_mask): bit0 drives animINST.state2 bit 0x80 (mirrors the entity's own
 *      camera-hide state when it has no entity yet), bit1 drives state2 bit 0x100 the same way;
 *   2. animINST.state bit 0 mirrors the Blam object's "is-locked-look" flag (HCEX_OBJ_FLAGS(obj) bit 30);
 *   3. the Halo->ws world transform — skipped (with a "way out of bounds" latch, state bit 0) when
 *      the converted position is absurdly large (>= 2,000,000 units), otherwise applied via
 *      animINST::Transform and propagated to every follower entity's offset transform;
 *   4. an active lip-sync update for this object, if one is registered in hcexLipSyncList;
 *   5. unless BOTH camera-visibility bits are set (fully hidden either way), the per-node
 *      local-transform recompute (from the fresh node matrices, or objCalcNodeLTM for nodes the
 *      model doesn't map), the resulting bounding-box/-OBB recompute (skippable via the
 *      dbg_hcex_off_bbox_calc debug toggle), and per-model-region permutation visibility
 *      (SetStateProcYes/No) driven by HCEX_MODEL's node-index and region tables.
 *
 * deviation: the two "hide/show from camera" vtable dispatches at raw slots 98/99 (decompiled as
 * bare 0-arg calls, `(*(void(**)(void))(vt+392))()`) are reproduced via direct vtable indexing
 * (entENTITY_vtblCall98/99) rather than a guessed named method, since they don't match the 1-arg
 * ShowToCamera/HideFromCamera slots hcex_light_sync uses. The per-follower ds::WEAK_PTR<entENTITY>
 * temporary's manual refcount bump/release (no behavioral effect within this single-threaded frame
 * step) is elided, matching this corpus's convention of not reproducing pure RAII bookkeeping. */

#include "../headers/hcex/hcex_obj_sync_boundary.h"

void hcex_obj_sync(HCEX_OBJ *obj)
{
    animINST *inst = obj->pInst;
    if ( !inst )
        return;

    unsigned int cam_mask = hcex_object_hidden_cam_mask(obj->id);
    int state2 = inst->state2;
    int hidden_bit0 = cam_mask & 1;
    int hidden_bit1 = (cam_mask >> 1) & 1;

    if ( ((state2 >> 7) & 1) != hidden_bit0 )
    {
        entENTITY *ent = inst->pEnt;
        if ( ent )
        {
            if ( hidden_bit0 )
                entENTITY_vtblCall98(ent);
            else
                entENTITY_vtblCall99(ent);
        }
        else
        {
            inst->state2 = hidden_bit0 ? (state2 | 0x80) : (state2 & ~0x80);
        }
    }

    if ( ((state2 >> 8) & 1) != hidden_bit1 )
    {
        entENTITY *ent = inst->pEnt;
        if ( ent )
        {
            if ( hidden_bit1 )
                entENTITY_vtblCall98(ent);
            else
                entENTITY_vtblCall99(ent);
        }
        else
        {
            inst->state2 = hidden_bit1 ? (inst->state2 | 0x100) : (inst->state2 & ~0x100);
        }
    }

    inst->state = (inst->state & ~1) | ((HCEX_OBJ_FLAGS(obj) >> 30) & 1);

    hcex_matr4x3 halo_matr;
    hcex_get_obj_matr(obj->id, &halo_matr);
    m3dMATR world_matr;
    hcex_make_inst_matr(&halo_matr, &world_matr);

    if ( fabsf(world_matr.elements[13]) >= 2000000.0f )
    {
        inst->state |= 1;
        return;
    }

    inst->Transform(&world_matr, 0);

    ds::WEAK_PTR<entENTITY> *followers = (ds::WEAK_PTR<entENTITY> *)obj->followers.pData;
    m3dMATR *followers_ofs = (m3dMATR *)obj->followersOfs.pData;
    for ( int i = 0; i < obj->followers.nElem; ++i )
    {
        ds::WEAK_PTR<entENTITY> *follower = &followers[i];
        if ( !follower->pHandle || !follower->pHandle->pPtr )
            continue;

        m3dMATR follower_matr = world_matr;
        follower_matr.RemoveScale();
        follower_matr.Transform(&followers_ofs[i], 1);

        entENTITY *follower_ent = follower->pHandle->pPtr;
        follower_ent->pInst->Transform(&follower_matr, 0);
    }

    dsCMP cmp = 0;
    int lip_sync_idx = dsVECTOR_lipsync_FindSorted(&hcexLipSyncList, &obj->id, &cmp);
    if ( lip_sync_idx != -1 )
        mdlLIP_SYNCER_ApplyLipSync(dsVECTOR_lipsync_index(&hcexLipSyncList, lip_sync_idx)->val);

    if ( cam_mask == 3 )
        return;   /* fully camera-hidden both ways: skip node/bbox/region-visibility recompute */

    hcex_matr4x3 node_matrices[64];
    unsigned char permutations[8];
    hcex_get_obj_node_matrices(obj->id, node_matrices, permutations);

    HCEX_MODEL *model = &hcexModels.pData[obj->modelIdx].val;

    float avg_scale = (inst->scale.x + inst->scale.y + inst->scale.z) * 0.33333334f;
    bool is_uniform_scale = fabsf(1.0f - avg_scale) < 0.000001f;

    m3dBOX total_bbox;
    total_bbox.CalcStart();

    bool skip_bbox_calc = dbg_hcex_off_bbox_calc.value;

    objITER_TREE iter(inst->pObj);
    objOBJ *cur = iter.pCur;
    while ( cur )
    {
        int node_index = (model->modelId == -1) ? -1 : model->nodeIndices.pData[cur->id];
        if ( node_index < 0 )
        {
            objCalcNodeLTM(cur);
        }
        else
        {
            hcex_make_matr(&node_matrices[node_index], &cur->matrLT);
            if ( !is_uniform_scale )
                cur->matrLT.Scale(avg_scale, avg_scale, avg_scale, 0);
        }

        if ( !skip_bbox_calc && (cur->state & (8 | 4)) == 0 )
        {
            m3dBOX node_box;
            if ( cur->pGeom && (cur->stateProc & 0x41100DF) == 0 )
            {
                cur->pGeom->pSharedGeom->bbox.Transform(&cur->matrLT, &node_box);
                total_bbox.CalcUpdate(&node_box);
            }
            else if ( (cur->state & (0x20000 | 0x40000)) != 0 )
            {
                node_box.bll.x = cur->matrLT.elements[12];
                node_box.bll.y = cur->matrLT.elements[13];
                node_box.bll.z = cur->matrLT.elements[14];
                _m3dCheckValid(&node_box.bll);
                node_box.fur = node_box.bll;
                node_box.Expand(0.1f);
                total_bbox.CalcUpdate(&node_box);
            }
        }

        iter.Next();
        cur = iter.pCur;
    }

    if ( total_bbox.Check() )
    {
        m3dOBB total_obb;
        total_bbox.ConvToOBB(&total_obb);

        inst->perInstLock.Lock(0, 0);
        inst->state2 |= 0x80000;
        *inst->GetBBox(1) = total_bbox;
        *inst->GetOBB(1) = total_obb;
        inst->perInstLock.Unlock(0, 0);
    }
    else
    {
        inst->state2 &= ~0x80000;
    }

    for ( int r = 0; r < model->regions.nElem; ++r )
    {
        HCEX_MODEL_REGION *region = &model->regions.pData[r];
        if ( (unsigned short)region->objRegId >= 0x8000u )
            continue;

        unsigned char active_sub_id = permutations[r];
        for ( int s = 0; s < region->objIds.nElem; ++s )
        {
            short sub_obj_id = region->objIds.pData[s];
            if ( (unsigned short)sub_obj_id >= 0x8000u )
                continue;

            objOBJ *sub_obj = inst->GetObj(sub_obj_id);
            if ( s == active_sub_id )
                sub_obj->SetStateProcNo(8, 0);
            else
                sub_obj->SetStateProcYes(8, 0);
        }
    }
}

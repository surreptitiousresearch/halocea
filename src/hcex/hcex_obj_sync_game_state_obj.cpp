/* hcex_obj_sync_game_state(HCEX_OBJ*) @0x823EC398 — one-arg overload: push every Blam "game state"
 * input this tracked object's ws-engine propHALO_OBJ property cares about into that property, once
 * per frame (see hcex_obj_sync_game_state_boundary.h for the full feature list). No-op if the
 * entity carries no properties at all, or no propHALO_OBJ property, or its descriptor's `objUse`
 * bitmask has nothing enabled.
 *
 * Named hcex_obj_sync_game_state_obj here (rather than the mangled name's bare hcex_obj_sync_game_state)
 * to keep it distinct from the 0-arg driver overload in hcex_obj_sync_game_state.c, which calls this
 * function once per tracked object.
 *
 * deviation: every "did this channel change" block in the original repeats the identical
 * box-old-and-new-as-dsDATA / share-and-copy-construct-a-sslOBJ_REF / HandleEvent / teardown dance
 * (only the prop-container float slot, the cached-value slot, and the propHALO_OBJ_DESC event-id slot
 * differ per channel); collapsed here into the `notify_channel_changed` helper. The raw "*(_DWORD*)(v18+88)"
 * bitmask tested throughout is propHALO_OBJ_DESC::objUse (apSTATE_T<unsigned int>, DB offset 0x58);
 * bit numbering below is inferred from which prop-container float slot / hcex_* query each block reads,
 * not from any DB enum (none is surfaced for this bitmask). */

#include "../headers/hcex/hcex_obj_sync_game_state_boundary.h"

/* Fire the object's SSL event `eventId` with the (new, old) boxed float values when they differ, then
 * remember `new_value` for next frame. Mirrors the repeated funcIn[]/funcOut[] change-notify blocks. */
static void notify_channel_changed(sslOBJ_REF *ssl_object, int event_id, float new_value, float *cached_value)
{
    if ( new_value == *cached_value )
        return;

    dsDATA new_data{}; /* dsDATA has ctors now: value-init, not aggregate */
    dsDATA old_data{};
    dsDATA_SetValue_float(&old_data, cached_value);
    dsDATA_SetValue_float(&new_data, &new_value);

    sslOBJ_REF ref_copy;
    sslOBJ_REF_copy_ctor(&ref_copy, ssl_object);
    sslOBJ_REF_HandleEvent(&ref_copy, event_id, &new_data, &old_data);
    sslOBJ_REF_dtor(&ref_copy);

    if ( new_data.type )
        dsDATA_TYPE_Destroy(new_data.type, &new_data);
    if ( old_data.type )
        dsDATA_TYPE_Destroy(old_data.type, &old_data);

    *cached_value = new_value;
}

void hcex_obj_sync_game_state_obj(HCEX_OBJ *obj)
{
    entENTITY_prop_view *entity = (entENTITY_prop_view *)obj->pInst->pEnt;

    if ( entity->propContainer.properties.nElem == 0 && entity->propContainer.components.nElem == 0 )
        return;

    propBASE *halo_prop_base = 0;

    if ( dsTYPE_ID_IsDerivedFrom(&entity->propContainer.typeId, &propHALO_OBJ_DESC_TYPE_ID) )
    {
        /* the container's own descriptor type IS propHALO_OBJ_DESC: propContainer itself is a
         * propHALO_OBJ at runtime (this entity's embedded prop storage was constructed as one). */
        halo_prop_base = (propBASE *)&entity->propContainer;
    }
    else if ( dsTYPE_ID_IsDerivedFrom(&entity->propContainer.typeId, &propCONTAINER_DESC_TYPE_ID) )
    {
        propBASE **properties = (propBASE **)entity->propContainer.properties.pData;
        for ( int i = 0; i < entity->propContainer.properties.nElem && !halo_prop_base; ++i )
            halo_prop_base = propBASE_GetProperty_HALO_OBJ(properties[i]);

        if ( !halo_prop_base )
        {
            propBASE **components = (propBASE **)entity->propContainer.components.pData;
            for ( int i = 0; i < entity->propContainer.components.nElem && !halo_prop_base; ++i )
                halo_prop_base = propBASE_GetProperty_HALO_OBJ(components[i]);
        }
    }
    else
    {
        return;
    }

    if ( !halo_prop_base )
        return;

    propHALO_OBJ_flat *prop = (propHALO_OBJ_flat *)halo_prop_base;
    /* propBASE::spDesc is a dsSMART_PTR; ::pointee is the raw descriptor pointer read here. */
    propHALO_OBJ_DESC_flat *desc = (propHALO_OBJ_DESC_flat *)prop->spDesc.pointee;
    if ( !desc->objUse.state )   /* objUse is apSTATE_T<unsigned long>; ::state is the bitmask */
        return;

    unsigned int use = desc->objUse.state;
    sslOBJ_REF *ssl_object = &prop->sslObject;

    /* bit0: machine powered on/off -> sml POWER_ON/POWER_OFF state. */
    if ( use & 0x1 )
    {
        int power_on = hcex_machine_is_power_on(obj->id);
        if ( ((HCEX_OBJ_FLAGS(obj) >> 26) & 1) != (unsigned)power_on )
        {
            HCEX_OBJ_FLAGS(obj) = (HCEX_OBJ_FLAGS(obj) & ~0x4000000u) | ((unsigned)power_on << 26);
            dsTSTRING_flat state_name = { 0 };
            dsTSTRING_UnsafeInit(&state_name, power_on ? "POWER_ON" : "POWER_OFF", -1, 0);
            sml_STATE_Set(&((entENTITY *)entity)->stateSml, &state_name);
            if ( --state_name.pBuffer->refCount == 0 )
                dlFree(state_name.pBuffer);
        }
    }

    /* bit1: active camouflage -> swap the entity's material to/from "act_camo". */
    if ( use & 0x2 )
    {
        int act_camo = hcex_obj_is_act_camo(obj->id);
        if ( ((HCEX_OBJ_FLAGS(obj) >> 29) & 1) != (unsigned)act_camo )
        {
            HCEX_OBJ_FLAGS(obj) = (HCEX_OBJ_FLAGS(obj) & ~0x20000000u) | ((unsigned)act_camo << 29);
            dsTSTRING_flat material = { 0 }, sub_material = { 0 };
            dsTSTRING_UnsafeInit(&material, act_camo ? "act_camo" : "", -1, 0);
            dsTSTRING_UnsafeInit(&sub_material, "", -1, 0);
            entENTITY_implSetMaterial((entENTITY *)entity, &material, &sub_material);
            if ( --sub_material.pBuffer->refCount == 0 )
                dlFree(sub_material.pBuffer);
            if ( --material.pBuffer->refCount == 0 )
                dlFree(material.pBuffer);
        }
    }

    /* bit2: shield-effect transparency fade on the "shield" sub-object. */
    if ( use & 0x4 )
    {
        float shield = hcex_shield_effect_on(obj->id);
        int shield_on = shield > 0.000001f;
        if ( ((HCEX_OBJ_FLAGS(obj) >> 28) & 1) != (unsigned)shield_on || shield > 0.000001f )
        {
            struct objOBJ *shield_obj = objFindName(animINST_GetObjRoot(obj->pInst), "shield");
            if ( shield_obj )
            {
                if ( ((HCEX_OBJ_FLAGS(obj) >> 28) & 1) != (unsigned)shield_on )
                {
                    if ( shield <= 0.000001f )
                        objOBJ_SetStateProcYes(shield_obj, 0x100000, 0);
                    else
                        objOBJ_SetStateProcNo(shield_obj, 0x100000, 0);
                    HCEX_OBJ_FLAGS(obj) = (HCEX_OBJ_FLAGS(obj) & ~0x10000000u) | ((unsigned)shield_on << 28);
                }
                objOBJ_SetTransparency(shield_obj, (int)(shield * 255.0f));
            }
        }
    }

    /* bit3: machine locked/unlocked -> hcexMachineLock/hcexMachineUnlock SSL script functions. */
    if ( use & 0x8 )
    {
        int locked = hcex_machine_is_locked(obj->id);
        if ( ((HCEX_OBJ_FLAGS(obj) >> 27) & 1) != (unsigned)locked )
        {
            HCEX_OBJ_FLAGS(obj) = (HCEX_OBJ_FLAGS(obj) & ~0x8000000u) | ((unsigned)locked << 27);
            sslOBJ_REF_CallFunc(ssl_object, locked ? &hcexMachineLock : &hcexMachineUnlock, 0, 0);
        }
    }

    /* bit4: "machine position" transparency (same fade dance as the shield effect). */
    if ( use & 0x10 )
    {
        float position = hcex_machine_position(obj->id);
        int position_on = position > 0.000001f;
        if ( ((HCEX_OBJ_FLAGS(obj) >> 25) & 1) != (unsigned)position_on || position > 0.000001f )
        {
            struct objOBJ *root_obj = animINST_GetObjRoot(obj->pInst);
            if ( root_obj )
            {
                if ( ((HCEX_OBJ_FLAGS(obj) >> 25) & 1) != (unsigned)position_on )
                {
                    if ( position <= 0.000001f )
                        objOBJ_SetStateProcYes(root_obj, 0x100000, 0);
                    else
                        objOBJ_SetStateProcNo(root_obj, 0x100000, 0);
                    HCEX_OBJ_FLAGS(obj) = (HCEX_OBJ_FLAGS(obj) & ~0x2000000u) | ((unsigned)position_on << 25);
                }
                objOBJ_SetTransparency(root_obj, (int)(position * 255.0f));
            }
        }
    }

    /* bits5-8: generic FUNC_A..D sml states, driven by Blam function channels 0..3. */
    static const char *const func_on_names[4]  = { "FUNC_A_ON",  "FUNC_B_ON",  "FUNC_C_ON",  "FUNC_D_ON"  };
    static const char *const func_off_names[4] = { "FUNC_A_OFF", "FUNC_B_OFF", "FUNC_C_OFF", "FUNC_D_OFF" };
    static const unsigned int func_bits[4] = { 24, 23, 22, 21 };
    for ( int i = 0; i < 4; ++i )
    {
        if ( (use & (0x20u << i)) == 0 )
            continue;

        float value;
        object_get_function_value(obj->id, (short)i, &value);
        int is_on = value > 0.000001f;
        if ( ((HCEX_OBJ_FLAGS(obj) >> func_bits[i]) & 1) != (unsigned)is_on )
        {
            HCEX_OBJ_FLAGS(obj) = (HCEX_OBJ_FLAGS(obj) & ~(1u << func_bits[i])) | ((unsigned)is_on << func_bits[i]);
            dsTSTRING_flat state_name = { 0 };
            dsTSTRING_UnsafeInit(&state_name, is_on ? func_on_names[i] : func_off_names[i], -1, 0);
            sml_STATE_Set(&((entENTITY *)entity)->stateSml, &state_name);
            if ( --state_name.pBuffer->refCount == 0 )
                dlFree(state_name.pBuffer);
        }
    }

    /* bit9: live entSFX_COLOR intensity, driven by function channel 1. */
    if ( use & 0x200 )
    {
        float intensity;
        object_get_function_value(obj->id, 1, &intensity);
        int num_sfx = entENTITY_GetNumSfx((entENTITY *)entity);
        for ( int i = 0; i < num_sfx; ++i )
        {
            entSFX *sfx = entENTITY_GetSfx((entENTITY *)entity, i);
            if ( entSFX_IsA_COLOR(sfx) )
                entSFX_COLOR_implSetIntensity((entSFX_COLOR *)sfx, intensity);
        }
    }

    /* bits10-13: SSL "function in" channels 0..3 (propHALO_OBJ::funcIn[0..3]). */
    if ( use & 0x400 )
        notify_channel_changed(ssl_object, desc->sslEventId_OnChangeInA,
                hcex_obj_get_function_in(obj->id, 0), &prop->funcIn[0]);
    if ( use & 0x800 )
        notify_channel_changed(ssl_object, desc->sslEventId_OnChangeInB,
                hcex_obj_get_function_in(obj->id, 1), &prop->funcIn[1]);
    if ( use & 0x1000 )
        notify_channel_changed(ssl_object, desc->sslEventId_OnChangeInC,
                hcex_obj_get_function_in(obj->id, 2), &prop->funcIn[2]);
    if ( use & 0x2000 )
        notify_channel_changed(ssl_object, desc->sslEventId_OnChangeInD,
                hcex_obj_get_function_in(obj->id, 3), &prop->funcIn[3]);

    /* bits14-17: SSL "function out" channels 0..3 (propHALO_OBJ::funcOut[0..3]). */
    for ( int i = 0; i < 4; ++i )
    {
        if ( (use & (0x4000u << i)) == 0 )
            continue;
        float value;
        object_get_function_value(obj->id, (short)i, &value);
        int event_id = (i == 0) ? desc->sslEventId_OnChangeOutA
                     : (i == 1) ? desc->sslEventId_OnChangeOutB
                     : (i == 2) ? desc->sslEventId_OnChangeOutC
                                : desc->sslEventId_OnChangeOutD;
        notify_channel_changed(ssl_object, event_id, value, &prop->funcOut[i]);
    }
}

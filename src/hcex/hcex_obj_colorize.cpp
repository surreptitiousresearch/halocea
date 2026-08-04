/* hcex_obj_colorize @0x823CE5F0 — recolor a tracked HCEX object's ws-engine model, but only when it is a
 * "grunt" instance. Looks up the HCEX_OBJ record, follows it to the animation instance and its root object,
 * and if the instance template name is exactly "grunt" applies the multiplayer hierarchy color. */

#include "../headers/hcex/hcex_obj_colorize_boundary.h"   /* HCEX_OBJ, animINST, objOBJ, hcex_obj_get, SetColorMPHier */

extern int strcmp(const char *a, const char *b);

extern "C" void hcex_obj_colorize(int id, unsigned int color)
{
    HCEX_OBJ *obj = hcex_obj_get(id);
    if ( !obj )
        return;

    animINST *inst = obj->pInst;
    if ( !inst )
        return;

    objOBJ *root_object = inst->pObj;
    if ( !root_object )
        return;

    const char *name = inst->nameTpl;
    if ( !name )
        name = empty_string;

    /* DEVIATION: the binary inlines the "grunt" comparison as a byte loop; it is a plain strcmp==0. */
    if ( strcmp(name, "grunt") == 0 )
        objOBJ_SetColorMPHier(root_object, color);
}

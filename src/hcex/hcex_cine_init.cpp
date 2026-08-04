/* hcex_cine_init @0x823B9C88 — one-time HCEX cinematic setup: register five native SSL script callbacks
 * on the global script system's global-object class, so HaloScript cinematics can call
 * SetHaloCineScreenEffectActive(), HcexStartCine(), HcexDontSkipCine(), AddCine() and
 * HideCineActorsByTplName(). Each registration follows the same pattern: resolve gsSslSystem's global
 * object, resolve its class, AddCbFunc the native callback under its script-visible signature, then
 * release the transient sslCLASS_REF/sslOBJ_REF/sslERROR values. */

#include "../headers/hcex/hcex_cine_init_boundary.h"

void hcex_cine_init(void)
{
    {
        sslOBJ_REF globalObj;
        sslSYSTEM_GetGlobalObj(&globalObj, gsSslSystem);
        sslCLASS_REF cls;
        sslOBJ_REF_GetClass(&cls, &globalObj);
        sslERROR err;
        sslCLASS_REF_AddCbFunc(&err, &cls, "SetHaloCineScreenEffectActive(isOn : bool)",
                                (void *)cbSetHaloCineScreenEffectActive, empty_string, 0);
        sslCLASS_REF_dtor(&cls);
        sslOBJ_REF_dtor(&globalObj);
    }
    {
        sslOBJ_REF globalObj;
        sslSYSTEM_GetGlobalObj(&globalObj, gsSslSystem);
        sslCLASS_REF cls;
        sslOBJ_REF_GetClass(&cls, &globalObj);
        sslERROR err;
        sslCLASS_REF_AddCbFunc(&err, &cls, "HcexStartCine(name : string = \"\")",
                                (void *)cbHcexStartCine, empty_string, 0);
        sslCLASS_REF_dtor(&cls);
        sslOBJ_REF_dtor(&globalObj);
    }
    {
        sslOBJ_REF globalObj;
        sslSYSTEM_GetGlobalObj(&globalObj, gsSslSystem);
        sslCLASS_REF cls;
        sslOBJ_REF_GetClass(&cls, &globalObj);
        sslERROR err;
        sslCLASS_REF_AddCbFunc(&err, &cls, "HcexDontSkipCine()",
                                (void *)cbHcexDontSkipCine, empty_string, 0);
        sslCLASS_REF_dtor(&cls);
        sslOBJ_REF_dtor(&globalObj);
    }
    {
        sslOBJ_REF globalObj;
        sslSYSTEM_GetGlobalObj(&globalObj, gsSslSystem);
        sslCLASS_REF cls;
        sslOBJ_REF_GetClass(&cls, &globalObj);
        sslERROR err;
        sslCLASS_REF_AddCbFunc(&err, &cls, "AddCine(object : object, name : string = \"\", mask : string = \"all\") ",
                                (void *)cbAddCine, empty_string, 0);
        sslCLASS_REF_dtor(&cls);
        sslOBJ_REF_dtor(&globalObj);
    }
    {
        sslOBJ_REF globalObj;
        sslSYSTEM_GetGlobalObj(&globalObj, gsSslSystem);
        sslCLASS_REF cls;
        sslOBJ_REF_GetClass(&cls, &globalObj);
        sslERROR err;
        sslCLASS_REF_AddCbFunc(&err, &cls, "HideCineActorsByTplName(name : string = \"\")",
                                (void *)cbHideActorsByTplName, empty_string, 0);
        sslCLASS_REF_dtor(&cls);
        sslOBJ_REF_dtor(&globalObj);
    }
}

/* hcex_cine_init @0x823B9C88 — one-time HCEX cinematic setup: register five native SSL script callbacks
 * on the global script system's global-object class, so HaloScript cinematics can call
 * SetHaloCineScreenEffectActive(), HcexStartCine(), HcexDontSkipCine(), AddCine() and
 * HideCineActorsByTplName(). Each registration follows the same pattern: resolve gsSslSystem's global
 * object, resolve its class, AddCbFunc the native callback under its script-visible signature, then
 * release the transient sslCLASS_REF/sslOBJ_REF/sslERROR values.
 *
 * DEVIATION (2026-08-07): the transient handles were previously declared as flat plain-C structs and
 * released through explicit sslCLASS_REF_dtor/sslOBJ_REF_dtor shim calls, because the boundary header
 * restated sslOBJ_REF/sslCLASS_REF locally. They are the canonical RAII ws-engine types now, so each
 * block's closing brace IS the release — which is what the binary emits, in this exact order:
 * AddCbFunc -> dlFree (the discarded sslERROR result's desc buffer) -> ~sslCLASS_REF -> ~sslOBJ_REF.
 * Copy-initialising from the by-value returns also removes the three phantom default constructions
 * the old spelling implied but the binary never performs. Same shape as hcex_library_init.cpp. */

#include "../headers/hcex/hcex_cine_init_boundary.h"

void hcex_cine_init(void)
{
    {
        sslOBJ_REF   globalObj = gsSslSystem->GetGlobalObj();
        sslCLASS_REF cls = globalObj.GetClass();
        cls.AddCbFunc("SetHaloCineScreenEffectActive(isOn : bool)",
                      cbSetHaloCineScreenEffectActive, empty_string, 0);
    }
    {
        sslOBJ_REF   globalObj = gsSslSystem->GetGlobalObj();
        sslCLASS_REF cls = globalObj.GetClass();
        cls.AddCbFunc("HcexStartCine(name : string = \"\")",
                      cbHcexStartCine, empty_string, 0);
    }
    {
        sslOBJ_REF   globalObj = gsSslSystem->GetGlobalObj();
        sslCLASS_REF cls = globalObj.GetClass();
        cls.AddCbFunc("HcexDontSkipCine()",
                      cbHcexDontSkipCine, empty_string, 0);
    }
    {
        sslOBJ_REF   globalObj = gsSslSystem->GetGlobalObj();
        sslCLASS_REF cls = globalObj.GetClass();
        cls.AddCbFunc("AddCine(object : object, name : string = \"\", mask : string = \"all\") ",
                      cbAddCine, empty_string, 0);
    }
    {
        sslOBJ_REF   globalObj = gsSslSystem->GetGlobalObj();
        sslCLASS_REF cls = globalObj.GetClass();
        cls.AddCbFunc("HideCineActorsByTplName(name : string = \"\")",
                      cbHideActorsByTplName, empty_string, 0);
    }
}

#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/ent/entCREATE_INFO.h"
#include "../../headers/ws/scn/scnSCENE.h"
#include "../../headers/entCREATE_DATA.h"

// Declared in entCreate__2.cpp (the scnSCENE/entCREATE_INFO& overload). Same free-function family.
entENTITY *entCreate(scnSCENE *pScn, entCREATE_INFO *info);

// entCreate(scnSCENE*, const char*, const char*, entCREATE_DATA*, msgDATA*) — @0x82536ED8
//
// Convenience overload: builds an entCREATE_INFO from the raw class/template/create-data/init
// arguments, then forwards to entCreate(scnSCENE*, entCREATE_INFO*).
entENTITY *entCreate(scnSCENE *pScn, const char *nameClass, const char *nameTpl,
                     entCREATE_DATA *pCD, msgDATA *pInitData)
{
    entCREATE_INFO info(nameClass, nameTpl, pCD, pInitData);
    return entCreate(pScn, &info);
}

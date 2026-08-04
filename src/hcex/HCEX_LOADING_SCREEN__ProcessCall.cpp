/* HCEX_LOADING_SCREEN::ProcessCall @0x823F44E8 — SSL script call dispatch: the only recognized
 * method name is "OnLoadingProgressChanged", which decodes paramList's first parameter as a
 * float into loadingScreenProgress.
 *
 * DEVIATION (PPC sret gotcha): the decompile's `dsPARAM::GetVal<float>(&v8, &paramList->list->id,
 * &this->loadingScreenProgress)` is the sret-shifted call in true register order --
 * this=paramList->list[0] (r4), sret dsSTRID result=a discarded local (r3), val=&loadingScreenProgress
 * (r5) -- confirmed against the disassembly (`addi r3,...var_10`/`lwz r4,4(r8)`/`addi r5, r3, 0x54`).
 * The string comparison the decompiler shows as a manual byte-by-byte loop is just strcmp(). */

#include "../headers/hcex/HCEX_LOADING_SCREEN.h"
#include "../headers/ws/ds/dsPARAM_LIST.h" // full dsPARAM_LIST/dsPARAM (paramList->list[0].GetVal)

extern "C" int strcmp(const char *a, const char *b); // boundary — CRT

void HCEX_LOADING_SCREEN::ProcessCall(const char *methodName, const dsPARAM_LIST *paramList)
{
    if (strcmp(methodName, "OnLoadingProgressChanged") == 0)
        paramList->list[0].GetVal(&loadingScreenProgress);
}

#include "../headers/ws/ssl/sslOBJ_REF.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/ws/ds/dsDATA_TYPE.h"
#include "../headers/ws/ds/dsTSTRING.h"
#include "../headers/ws/ds/dsVECTOR.h"

// cinematic-actor hide list, keyed by template name; the tick path consumes and clears it.
extern dsVECTOR<dsTSTRING<char>, 8> cineHiddenActorsTplNames; // DB dsVECTOR<dsTSTRING<char>,8>

// cbHideActorsByTplName @ 0x823B7920 — SSL callback "HideCineActorsByTplName(name : string = \"\")".
// Queue a (non-empty) template name so the cinematic tick hides every matching actor.
extern "C" void cbHideActorsByTplName(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal,
                                      sslOBJ_REF caller)
{
    (void)argc;
    (void)retVal;
    // GetPtr yields the boxed argument's dsTSTRING<char>*; take a refcount-shared copy.
    dsTSTRING<char> *nameArg =
        argv->type ? reinterpret_cast<dsTSTRING<char> *>(argv->type->GetPtr(&argv->storage)) : nullptr;

    dsTSTRING<char> name = *nameArg;
    if (name.pBuffer->strLen)
        cineHiddenActorsTplNames.PushBack(name); // PushBack takes const dsTSTRING<char>& (not a pointer)
    // `name` released by its destructor on return.
}

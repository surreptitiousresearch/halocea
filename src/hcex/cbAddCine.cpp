#include "../headers/hcex/hcex_add_cine_boundary.h"
#include <string.h> // strcmp

// cbAddCine @ 0x823B97D8 — SSL callback "AddCine(name : string = "", mask : string = "")". Register the
// calling object's cinematic actor into the in-progress cinematics registry `listCine`, under a name
// (an explicit override argument, else the actor's own name) and a difficulty bitmask parsed from the
// optional "mask" argument ("all" or a '|'-separated list of easy/normal/hard/impossible; absent => all).
// Source: D:\Projects\code\HCEX\sources\halo_cine.cpp.
extern "C" void cbAddCine(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller)
{
    (void)retVal;

    if (argc < 1)
        return;

    // arg0: the cinematic object -> its callback host, reinterpreted as the cinematic actor.
    sslOBJ_REF *objRef =
        argv[0].type ? reinterpret_cast<sslOBJ_REF *>(argv[0].type->GetPtr(&argv[0].storage)) : nullptr;
    gsANITEC_ACTOR *actor = reinterpret_cast<gsANITEC_ACTOR *>(sslOBJ_REF_bridge::GetCbHost(objRef));
    if (!actor)
        return;

    // Default name: the actor's own display name (decompiler dispatches this through the sslCB_HOST
    // vtable slot 12; on a gsANITEC_ACTOR that is entENTITY::GetName).
    dsTSTRING<char> name = actor->GetName();

    // arg1 (optional): explicit name override, applied only when non-empty.
    if (argc >= 2)
    {
        dsTSTRING<char> *nameArg =
            argv[1].type ? reinterpret_cast<dsTSTRING<char> *>(argv[1].type->GetPtr(&argv[1].storage))
                         : nullptr;
        dsTSTRING<char> nameOverride = *nameArg; // refcount-shared copy
        if (nameOverride.pBuffer->strLen)
            name = nameOverride;
    }

    // arg2 (optional): difficulty mask. Absent => all difficulties (0xFF).
    unsigned char mask = 0;
    if (argc >= 3)
    {
        dsTSTRING<char> *maskArg =
            argv[2].type ? reinterpret_cast<dsTSTRING<char> *>(argv[2].type->GetPtr(&argv[2].storage))
                         : nullptr;
        dsTSTRING<char> maskStr = *maskArg; // refcount-shared copy

        if (strcmp(maskStr.pBuffer->str, "all") == 0)
        {
            mask = 0xFF;
        }
        else
        {
            // Split on '|' and OR in one bit per recognized difficulty token.
            dsVECTOR<dsTSTRING<char>, 8> tokens;
            tokens.pData = nullptr;
            tokens.nElem = 0;
            tokens.allocated = 0;

            dsTSTRING<char> delim = "|";
            ds::Tokenize<ds::DEF_TOKEN_PROCESSOR>(&maskStr, &delim,
                                                  reinterpret_cast<ds::DEF_TOKEN_PROCESSOR *>(&tokens));

            for (int i = 0; i < tokens.nElem; ++i)
            {
                const char *tok = tokens[i].pBuffer->str;
                int bit;
                if (strcmp(tok, "easy") == 0)
                    bit = 0;
                else if (strcmp(tok, "normal") == 0)
                    bit = 1;
                else if (strcmp(tok, "hard") == 0)
                    bit = 2;
                else if (strcmp(tok, "impossible") == 0)
                    bit = 3;
                else
                    continue; // unrecognized token contributes no bit
                mask |= (unsigned char)(1 << bit);
            }
        }
    }
    else
    {
        mask = 0xFF;
    }

    // Ensure a registry entry for `name` exists (INS_DUP_IGNORE keeps any existing one), then append
    // this actor to its per-difficulty table if not already present.
    dsVECTOR<hcexCINE_INFO, 8> emptyActors;
    emptyActors.pData = nullptr;
    emptyActors.nElem = 0;
    emptyActors.allocated = 0;

    hcex_cine_entry entry = { name, emptyActors }; // dsPAIR(const K&, const V&)
    dsCMP cmp;
    int inserted = listCine.InsertSorted<dsCMP>(entry, cmp,
                                                dsVECTOR<hcex_cine_entry, 8>::INS_DUP_IGNORE);

    dsVECTOR<hcexCINE_INFO, 8> *actors = &listCine[inserted].val;

    bool alreadyPresent = false;
    for (int i = 0; i < actors->nElem; ++i)
    {
        if ((*actors)[i].actor == actor)
        {
            alreadyPresent = true;
            break;
        }
    }
    if (!alreadyPresent)
    {
        // The binary builds the record inline (actor@0, mask byte@4) rather than via the addressed
        // hcexCINE_INFO ctor; the (actor, mask) ctor is its memberwise equivalent.
        apSTATE_T<unsigned char> maskState;
        maskState.state = mask; // apSTATE_T<unsigned char>::state — the difficulty bitmask byte
        hcexCINE_INFO info(actor, maskState);
        actors->PushBack(info);
    }
    // `name`, `entry` and the scratch vectors are released by their destructors on return.
}

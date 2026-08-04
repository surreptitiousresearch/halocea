// ?Init@txmTEX_DESC_DB@@QAAXXZ  (0x82C66C24)
// One-shot lazy load of the 10 default texture descriptors: index 0 from "default_texdesc.td",
// indices 1..9 from "default_texdesc_<TEX_SUFFIXES_1[i]>.td". Each file is parsed via the ".ps"
// script loader and handed to txmTexDescLoad; on any parse failure the slot falls back to
// ENGINE_TEX_DESC::Default(). No-op once inited.
//
// The decompiler renders this as a dense dsTSTRING refcount dance (inlined copy-ctor / operator= /
// destructor bodies); reconstructed here with the ordinary dsTSTRING value operations the source
// used. The psSYSTEM::LoadPsFromFile call is a boundary (see txm_texdesc_db_init_boundary.h note
// on the sret/root argument artifact).
// Source: D:\Projects\code\common\src.sys ... (txm subsystem)

#include "../../headers/ws/txm/txmTEX_DESC_DB.h"
#include "../../headers/ws/txm/txm_texdesc_db_init_boundary.h"

void txmTEX_DESC_DB::Init()
{
    if (this->inited)
        return;

    for (int i = 0; i < 10; ++i) {
        dsTSTRING<char> fileName;

        if (i) {
            // "default_texdesc_" + TEX_SUFFIXES_1[i] + ".td"
            dsTSTRING<char> suffix(TEX_SUFFIXES_1[i]);
            dsTSTRING<char> base("default_texdesc_");
            dsTSTRING<char> combined = base + suffix;
            combined.Insert(combined.pBuffer->strLen, ".td", -1);
            fileName = combined;
        } else {
            dsTSTRING<char> only("default_texdesc.td");
            fileName = only;
        }

        // subDir / extension are empty (the filename already carries the ".td" extension).
        dsTSTRING<char> emptySubDir;
        dsTSTRING<char> emptyExt;

        psSECTION section = psSYSTEM__LoadPsFromFile(&psSystem, fileName, emptySubDir, emptyExt, 0);
        if (!txmTexDescLoad(&this->default_desc[i], &section))
            this->default_desc[i] = ENGINE_TEX_DESC::Default();
        section.~psSECTION();
    }

    this->inited = true;
}

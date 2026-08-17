/* ?Load@txmMANAGER@@QAAHPAVtxmTEXTURE@@H@Z @0x826F71F0 */
#include "../../headers/ws/txm/txmMANAGER.h"

extern void _apLog(const char *format, ...);              // boundary -- Saber logging sink (?_apLog@@YAXPBDZZ)
extern char *fnmGetPath(const char *name, char *outBuffer); // boundary -- filename/path utility, not reversed

/* 0x826F71F0 -- txmMANAGER::Load(txmTEXTURE *pTex, int isTryLoadNotFound)
 *
 * Loads (or confirms already-loaded/pending) pixel data for a single texture: skips work if the
 * texture's state already reports loaded/loading/not-found (bits 21, 23, and the 0x120 pair,
 * exact per-bit meaning not further reversed here); otherwise resolves the texture's disk path
 * via txmTEXTURE::FindDisk against this manager's search path/extension, normalizes it through
 * fnmGetPath, and hands the normalized path to txmTEXTURE::Load. Logs and marks the texture
 * not-found (state bit 0x800000) if FindDisk fails to locate it.
 *
 * isTryLoadNotFound is accepted but unused by this function body, matching the original. */
int txmMANAGER::Load(txmTEXTURE *pTex, int isTryLoadNotFound)
{
    if ( !pTex )
        return 0;

    unsigned int texState = pTex->state.state;

    if ( ((texState >> 21) & 1) != 0 || (texState & 0x120) != 0 || ((texState >> 23) & 1) != 0 )
        return 1;

    char diskPath[272];  // resolved-path buffer (FindDisk's output, later overwritten with the
                         // fnmGetPath-normalized copy)
    char normalizedPathScratch[520]; // fnmGetPath's scratch output buffer

    if ( pTex->FindDisk(this->path, this->ext, diskPath) )
    {
        fnmGetPath(diskPath, normalizedPathScratch);

        // Byte-copy the normalized path back over diskPath (matches the disasm's manual
        // NUL-terminated copy loop rather than a library call).
        int i = 0;
        char c;
        do
        {
            c = normalizedPathScratch[i];
            diskPath[i++] = c;
        }
        while ( c );

        return pTex->Load(diskPath, this->ext) != 0;
    }
    else
    {
        _apLog("~Texture,Error~Cannot locate texture %s", pTex->name.pBuffer->str);
        pTex->state.state |= 0x800000u;
        return 0;
    }
}

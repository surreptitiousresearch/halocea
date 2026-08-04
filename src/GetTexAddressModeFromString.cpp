/* GetTexAddressModeFromString @0x82C65A58 — map a texture-address-mode name string
 * ("wrap"/"clamp"/"mirror", case-insensitive) to its ENGINE_TEX_ADDRESS_MODE enum value.
 * Linear scan of the parallel name table ENGINE_TEX_ADDRESS_MODE_STR. Returns 1 and writes
 * *mode on a hit; returns 0 (leaving *mode untouched) if the string matches nothing. */

#include "headers/ws/txm/ENGINE_TEX_DESC.h" // ENGINE_TEX_ADDRESS_MODE (ETADDR_WRAP..ETADDR_LAST)

/* Parallel string table, one entry per ENGINE_TEX_ADDRESS_MODE value [0, ETADDR_LAST).
 * Data boundary — defined in the rasterizer string-tables translation unit. */
extern const char *ENGINE_TEX_ADDRESS_MODE_STR[];

extern "C" int stricmp(const char *a, const char *b); // CRT boundary (case-insensitive compare)

int GetTexAddressModeFromString(ENGINE_TEX_ADDRESS_MODE *mode, const char *str)
{
    ENGINE_TEX_ADDRESS_MODE candidate = ETADDR_WRAP;
    const char **entry = ENGINE_TEX_ADDRESS_MODE_STR;

    while (stricmp(*entry, str))
    {
        ++entry;
        candidate = (ENGINE_TEX_ADDRESS_MODE)(candidate + 1);
        // DEVIATION: the binary tests `entry == &M3D_EPSILON_2_293` — the address of the global
        // immediately following the table, i.e. one-past-end == &ENGINE_TEX_ADDRESS_MODE_STR[ETADDR_LAST].
        if (entry == &ENGINE_TEX_ADDRESS_MODE_STR[ETADDR_LAST])
            return 0;
    }

    *mode = candidate;
    return 1;
}

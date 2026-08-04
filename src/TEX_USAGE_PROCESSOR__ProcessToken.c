#include "headers/ws/txm/TEX_USAGE_PROCESSOR.h"

// 0x82C65BB8 -- matches `token` against the ENGINE_MAP_TYPE_NAME string table (terminated by
// the ENGINE_TEX_ADDRESS_MODE_STR table that immediately follows it in the data segment); on a
// match, ORs bit (1 << matchIndex) into usage.maps and bumps usage.numMaps. Returns
// TOKENIZE_ERROR if `token` matches nothing in the table (length is unused -- the table entries
// are NUL-terminated and compared with a plain strcmp-style scan).
ds::TOKENIZE_RESULT TEX_USAGE_PROCESSOR::ProcessToken(const char *token, int length)
{
    int matchIndex = 0;
    for (const char **entry = ENGINE_MAP_TYPE_NAME; entry != ENGINE_TEX_ADDRESS_MODE_STR; ++entry, ++matchIndex)
    {
        const char *tableStr = *entry;
        const char *tokenStr = token;
        while (*tokenStr == *tableStr && *tokenStr != 0)
        {
            ++tokenStr;
            ++tableStr;
        }
        if ((unsigned char)*tokenStr == (unsigned char)*tableStr)
        {
            this->usage.maps |= (1u << matchIndex);
            this->usage.numMaps += 1;
            return ds::TOKENIZE_CONTINUE;
        }
    }

    return ds::TOKENIZE_ERROR;
}

#include "headers/ws/txm/txm_functions.h"
#include "headers/ws/txm/TEX_USAGE_PROCESSOR.h"

// 0x82C6655C -- tokenizes `usage_str` on ':'/'+' delimiters via ds::Tokenize<TEX_USAGE_PROCESSOR>,
// writing the accumulated usage bitmask to *usage. Returns false (leaving *usage untouched) on a
// tokenize error.
bool txmParseTexUsage(ENGINE_TEX_DESC::USAGE &usage, const char *usage_str)
{
    dsTSTRING<char> delims;
    delims.UnsafeInit(":+", -1, 0);

    dsTSTRING<char> str;
    str.UnsafeInit(usage_str, -1, 0);

    TEX_USAGE_PROCESSOR processor;
    processor.currentContext = TEX_USAGE_PROCESSOR::TEX_MAPS;
    processor.usage.maps = 0;
    processor.usage.numMaps = 0;

    bool ok = ds::Tokenize<TEX_USAGE_PROCESSOR>(&str, &delims, processor) != ds::TOKENIZE_ERROR;

    if (!ok)
        return false;

    usage = processor.usage;
    return true;
}

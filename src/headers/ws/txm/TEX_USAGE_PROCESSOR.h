#pragma once
#include "ENGINE_TEX_DESC.h"
#include "../ds/dsTOKENIZE_RESULT.h"
// ws-engine txm: ds::Tokenize<T> callback object that parses a ':'+'-delimited "usage" string
// (e.g. "diffuse+normal+specular") from a .td script into an ENGINE_TEX_DESC::USAGE bitmask.
// Driven by txmParseTexUsage via ds::Tokenize<TEX_USAGE_PROCESSOR>.
// DB-verified layout (types_members TEX_USAGE_PROCESSOR): currentContext@0 (CONTEXT enum),
// usage@4 (ENGINE_TEX_DESC::USAGE, 8B) -- size 12.

typedef struct TEX_USAGE_PROCESSOR
{
    // DB-verified (types_enum_values TEX_USAGE_PROCESSOR::CONTEXT): TEX_MAPS=0 (only value seen).
    enum CONTEXT
    {
        TEX_MAPS = 0,
    } currentContext; /* 0x00 */

    ENGINE_TEX_DESC::USAGE usage; /* 0x04 */

    // 0x82C65BB8 -- reversed in TEX_USAGE_PROCESSOR__ProcessToken.c. Called once per token by
    // ds::Tokenize<TEX_USAGE_PROCESSOR>; matches `token` against the ENGINE_MAP_TYPE_NAME string
    // table and, on a hit, ORs the corresponding bit into usage.maps and bumps usage.numMaps.
    ds::TOKENIZE_RESULT ProcessToken(const char *token, int length);

    // 0x82C65C08 -- delimiter callback (':'/'+' etc). Body external to this batch. boundary.
    ds::TOKENIZE_RESULT ProcessDelim(char delim);

    // 0x82C65B98 -- accessor for `usage`. Body external to this batch. boundary.
    const ENGINE_TEX_DESC::USAGE &GetUsage() const;
} TEX_USAGE_PROCESSOR;

// String tables consulted by ProcessToken (ENGINE_MAP_TYPE_NAME[0..]) and by txmTexDescLoad's
// address-mode parsing (ENGINE_TEX_ADDRESS_MODE_STR -- the sentinel one-past ENGINE_MAP_TYPE_NAME).
// Not reversed as data in this batch -- boundary raw string-table pointers.
extern "C" const char *ENGINE_MAP_TYPE_NAME[];
extern "C" const char *ENGINE_TEX_ADDRESS_MODE_STR[];

// 0x82C66558 -- parse `str` ("wrap"/"clamp"/"mirror") into *mode. Body not decompiled in this
// batch (called from txmTexDescLoad's address_u/address_v handling). boundary.
extern "C" void GetTexAddressModeFromString(ENGINE_TEX_ADDRESS_MODE *mode, const char *str);

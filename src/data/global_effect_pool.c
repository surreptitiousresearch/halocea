/* global_effect_pool @ 0x84429928 (.data, 4 bytes)
 * DB applied_types: ID3DXEffectPool *global_effect_pool;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 * global_effect_pool — global D3DX effect pool pointer (data-bss, 4)
 */
#include "../headers/ID3DXEffectPool.h"

ID3DXEffectPool *global_effect_pool;

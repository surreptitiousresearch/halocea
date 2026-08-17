/* ?IsSuccess@haloSTORAGE_TASK@@UAA_NXZ @0x823F49C0 */
#include "../headers/hcex/haloSTORAGE_TASK.h"

// 0x823F4A00
bool haloSTORAGE_TASK::IsSuccess()
{
    return isReady && !error;
}

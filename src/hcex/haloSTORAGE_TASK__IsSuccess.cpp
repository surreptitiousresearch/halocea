#include "../headers/hcex/haloSTORAGE_TASK.h"

// 0x823F4A00
bool haloSTORAGE_TASK::IsSuccess()
{
    return isReady && !error;
}

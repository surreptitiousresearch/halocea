/* hcexCINE_INFO::hcexCINE_INFO @0x823B2B80 — record the actor and snapshot its current state byte
 * into `mask`. */

#include "../../headers/hcex/hcexCINE_INFO.h"

hcexCINE_INFO::hcexCINE_INFO(gsANITEC_ACTOR *actor, const apSTATE_T<unsigned char> &mask)
{
    this->actor = actor;
    this->mask = mask;
}

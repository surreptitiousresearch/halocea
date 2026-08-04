/* HCEX_LOADING_SCREEN_DESC::~HCEX_LOADING_SCREEN_DESC @0x823F4140 — chain to ~sslCLASS_REF then
 * (implicitly, via the base destructor at scope exit) ~dscDESC. */

#include "../headers/hcex/HCEX_LOADING_SCREEN_DESC.h"

HCEX_LOADING_SCREEN_DESC::~HCEX_LOADING_SCREEN_DESC()
{
    sslClass.~sslCLASS_REF();
}

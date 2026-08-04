#include "../headers/ws/ssl/sslOBJ_REF.h"
#include "../headers/ws/ds/dsDATA.h"
#include "../headers/blam_data_globals.h" // hcex_dont_skip_cine

// cbHcexDontSkipCine @ 0x823B2C90 — SSL callback "HcexDontSkipCine()". Latch the flag that keeps the
// current cinematic from being skipped by player input.
extern "C" void cbHcexDontSkipCine(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal,
                                   sslOBJ_REF caller)
{
    (void)argc;
    (void)argv;
    (void)retVal;
    hcex_dont_skip_cine = 1;
}

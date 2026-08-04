#include "../../headers/ws/gs/gsUSER_CFG_MNG.h"

// 0x823D29E0 — return the primary user's apCONFIG handle. Thin forwarder onto Get(int) using the
// user manager's primaryUserIdx. (The disassembly confirms r3 is the sret result handle and r4 is
// `this`; the decompiler's this/result labels were transposed.)
ds_data::REF_TYPE<apCONFIG> gsUSER_CFG_MNG::Get()
{
    return Get(gsUserMng->primaryUserIdx);
}

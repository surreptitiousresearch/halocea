#pragma once
// ws-engine msg: outcome of msgSYSTEM::SendMsg / msgSYSTEM::Dispatch. DB-verified enumerators
// (types_enum_values msgDSP_RES).

enum msgDSP_RES {
    MSG_DSP_RES_SENT     = 0, // message was dispatched to the recipient
    MSG_DSP_RES_NOT_SENT = 1, // recipient not registered / suspended / filtered out
    MSG_DSP_RES_ERROR    = 2, // dispatch failed
};

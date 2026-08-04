#pragma once
// ws-engine msg: result of an msgADDR::ProcessMsg dispatch. DB-verified enumerators
// (types_enum_values msgRES).

enum msgRES {
    MSG_RES_ABORT    = 0, // handler consumed the message; stop propagation
    MSG_RES_CONTINUE = 1, // continue dispatching to further handlers
    MSG_RES_ERROR    = 2, // dispatch failed
};

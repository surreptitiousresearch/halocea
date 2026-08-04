#pragma once

typedef enum net_graph_data_type /* : __int32 — enum-base is C++; underlying width 4 preserved (values fit int) */
{
    net_graph_data_type_bytes      = 0x0,
    net_graph_data_type_packets    = 0x1,
    NUMBER_OF_NET_GRAPH_DATA_TYPES = 0x2,
    net_graph_data_type_invalid    = -1,
} net_graph_data_type;

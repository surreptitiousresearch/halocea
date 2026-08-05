#pragma once
/* move_position_order — the order in which an actor walks the move positions of its encounter
 * squad (alert_state_data.move_position_order; the move_position_order argument of
 * action_alert_setup; the values of global_state_move_position_orders @0x82128A10).
 * DB-AUTHORITATIVE: types_enum_values $D221D3C7D52C83236971CA5D6C9E8070. */

enum move_position_order
{
    _move_position_order_none = 0,
    _move_position_order_repeat = 1,
    _move_position_order_loop = 2,
    _move_position_order_loop_back_and_forth = 3,
    _move_position_order_loop_randomly = 4,
    _move_position_order_random = 5,
    NUMBER_OF_MOVE_POSITION_ORDERS = 6,
};

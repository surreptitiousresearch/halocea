/* effects_disconnect_from_structure_bsp @0x836E1208 — hook invoked when the active structure BSP is torn
 * down (BSP switch). In this build the body is empty: live effects carry no cached structure-BSP state that
 * must be released on disconnect (their reconnect-side counterpart, effects_reconnect_to_structure_bsp,
 * does all the work). Preserved as an explicit no-op so the disconnect proc table stays symmetric. */

void effects_disconnect_from_structure_bsp(void)
{
}

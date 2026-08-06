/* reference_values @ 0x8211576C (.rdata, 16 bytes)
 * DB applied_types: const float reference_values[4];
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   +0x0000 = 0x3F490FDB
 *   +0x0004 = 0x4016CBE4
 *   +0x0008 = 0xBF490FDB
 *   +0x000C = 0xC016CBE4
 * .rdata, 4 x float. The per-quadrant diagonal reference angles
 * the legacy joystick presets subtract from the raw stick angle before the snap test in
 * src/input_mode_handle_game_xbox.c (quadrant = (y < 0 ? 2 : 0) | (x < 0)). Initializer
 * reconstructed from the binary (big-endian words):
 *   +0x0000 0x3F490FDB ->  0.78539819f =  pi/4    (quadrant 0: x >= 0, y >= 0)
 *   +0x0004 0x4016CBE4 ->  2.3561945f  =  3*pi/4  (quadrant 1: x <  0, y >= 0)
 *   +0x0008 0xBF490FDB -> -0.78539819f = -pi/4    (quadrant 2: x >= 0, y <  0)
 *   +0x000C 0xC016CBE4 -> -2.3561945f  = -3*pi/4  (quadrant 3: x <  0, y <  0)
 * Each literal is the bit-exact round trip of the stored word (and of PI/4 and PI*0.75 computed
 * from math_constants.h PI). The ledger extent of 20 is the next-named-symbol gap
 * (control_to_name_table @0x82115780); the four trailing zero bytes are alignment. The true
 * element count is 4 — the consumer indexes quadrants 0..3 and declares `[4]`, which matches.
 */
const float reference_values[4] = { 0.78539819f, 2.3561945f, -0.78539819f, -2.3561945f };

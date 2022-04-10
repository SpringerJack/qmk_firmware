/* Copyright 2017 Wunder
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * In case of bricking-emergency you can enter bootloader mode by
 * pressing Space+b while plugging the mech back in
 */

#include QMK_KEYBOARD_H
#include "keymap_german.h"
#include "keymap_neo2.h"

enum layer_names { _DE, _1, _2, _3, _4, _5, _6, _7, _8, _9, _vim };
// int shifted = 0;
// bool key(uint16_t k1, uint16_t k2);

enum my_keycodes { QWERTZ, CUSTOM, DELWORD };

/* Tap Dance Config */
/* generic code */
typedef enum {
  TD_NONE,
  TD_UNKNOWN,
  TD_SINGLE_TAP,
  TD_TAP_HOLD,
  TD_DOUBLE_TAP,
  TD_SINGLE_HOLD
} td_state_t;

typedef struct {
  bool is_press_action;
  td_state_t state;
} td_tap_t;

enum { X_UPPER_THUMB };

static td_tap_t xtap_state = {.is_press_action = 1, .state = TD_NONE};

/* Functions for sending custom keycodes within a tap dance */
static keyrecord_t dummy_record = {
    .event =
        {
            .key =
                {
                    .col = 0,
                    .row = 0,
                },
            .pressed = 0,
            .time = 0,
        },
    .tap = {0},
};

// Filling the dummy_record for processing with `process_record_kb`
void setup_dummy_record(uint8_t col, uint8_t row, bool pressed) {
  dummy_record.event.key.col = col;
  dummy_record.event.key.row = row;
  dummy_record.event.pressed = pressed;
  dummy_record.event.time = timer_read();
}

// Register a custom defined keycode with `process_record_kb`
void register_custom_keycode(uint16_t keycode, uint8_t col, uint8_t row) {
  setup_dummy_record(col, row, true);
  process_record_kb(keycode, &dummy_record);
}

// Unregister a custom defined keycode with `process_record_kb`
void unregister_custom_keycode(uint16_t keycode, uint8_t col, uint8_t row) {
  setup_dummy_record(col, row, false);
  process_record_kb(keycode, &dummy_record);
}

// Tapping a custom keycode
void tap_custom_keycode(uint16_t keycode, uint8_t col, uint8_t row) {
  register_custom_keycode(keycode, col, row);
  wait_ms(10);
  unregister_custom_keycode(keycode, col, row);
}
/* End functions for sending custom keycodes within a tap dance */

td_state_t get_dance(qk_tap_dance_state_t *state) {
  bool tap_action = state->interrupted || !state->pressed;
  switch (state->count) {
  case 1:
    return tap_action ? TD_SINGLE_TAP : TD_SINGLE_HOLD;
  case 2:
    return tap_action ? TD_DOUBLE_TAP : TD_TAP_HOLD;
  default:
    return TD_UNKNOWN;
  }
}

/* Tap Dance for Button 3 == Upper Thumb */
void x_finished_btn3(qk_tap_dance_state_t *state, void *user_data) {
  xtap_state.state = get_dance(state);
  switch (xtap_state.state) {
  default:
    return;
  }
}

void x_reset_btn3(qk_tap_dance_state_t *state, void *user_data) {
  switch (xtap_state.state) {
  default:
    return;
  }
  xtap_state.state = TD_NONE;
}
/* End Tap Dance for Button 3 */

/* Tap Dance Keys */
qk_tap_dance_action_t tap_dance_actions[] = {
    [X_UPPER_THUMB] =
        ACTION_TAP_DANCE_FN_ADVANCED(NULL, x_finished_btn3, x_reset_btn3)};
/* End Tap Dance Config */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* standard DE Layout
     * Composing keys: KC_RBRC, KC_GRV, KC_EQL
     */
    [_DE] = LAYOUT_ortho_5x15(

        KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_INS, KC_DEL, KC_PSCR, KC_6,
        KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T,
        KC_VOLD, KC_MUTE, KC_VOLU, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC,
        KC_CLCK, KC_A, KC_S, KC_D, KC_F, KC_G, KC_NO, KC_NO, C(KC_BSPC), KC_H,
        KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_LSFT, KC_Z, KC_X, KC_C, KC_V,
        KC_B, KC_COPY, KC_PSTE, KC_NO, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH,
        KC_RSFT, KC_LCTL, KC_LGUI, KC_LALT, KC_CLCK, LT(_4, KC_MINS), KC_SPC,
        MO(_vim), TG(_7), KC_RCTL, KC_ENT, LT(_4, KC_0), KC_NUHS, KC_LGUI,
        KC_MINS, OSL(_9)),

    /* Neo-Qwertz
     * .-----------------------------------------------------------------------------------------------------------------------------.---------
     * | Esc    | 1   °  | 2   §  | 3   ℓ  | 4   »  | 5   «  | INSERT | DELETE |
     * 6  $   | 7    € | 8   „  | 9     “| 0    ” | ß   ẞ  | Back   |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|--------+
     * |  Tab   | Q      | W      | E      | R      | T      | VolM   | PRINT  |
     * Z      | U      | I      | O      | P      | ü      | ̂       |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|--------+
     * |  Mod3  | A      | S      | D      | F      | G      | Vol-   | Vol+   |
     * H      | J      | K      | L      | Ö      | ä      | ̀       |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|--------+
     * | LShift | Y      | X      | C      | V      | B      | Copy   | Paste  |
     * N      | M      | , ;    | . :    | - —    | RShift |  ́      |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|--------+
     * | Ctrl   |  Super |  LAlt  | Mod3   |  Mod4  | Space  | F-Layer| Mouse-L|
     * Enter  | Mod3   | Mod4   | Super  | Esc    |  RCtl  | FN     |
     * '-----------------------------------------------------------------------------------------------------------------------------'---------
     */
    [_1] = LAYOUT_ortho_5x15(
        KC_ESC, NE_1, NE_2, NE_3, NE_4, NE_5, KC_INS, KC_DEL, KC_BSPC, NE_6,
        NE_7, NE_8, NE_9, NE_0, DE_SS, KC_TAB, DE_Q, DE_W, DE_E, DE_R, DE_T,
        KC_MUTE, KC_PSCR, NE_GRV, DE_Z, DE_U, DE_I, DE_O, DE_P, DE_UDIA, MO(_3),
        DE_A, DE_S, DE_D, DE_F, DE_G, KC_VOLD, KC_VOLU, NE_ACUT, DE_H, DE_J,
        DE_K, DE_L, DE_ODIA, DE_ADIA, KC_LSFT, DE_Y, DE_X, DE_C, DE_V, DE_B,
        KC_COPY, KC_PSTE, NE_CIRC, DE_N, DE_M, DE_COMM, DE_DOT, NE_MINS,
        KC_RSFT, KC_LCTL, KC_LGUI, KC_LALT, MO(_3), LT(_4, KC_MINS), KC_SPC,
        KC_TRNS, TG(_7), MO(_8), KC_ENTER, MO(_3), MO(_4), KC_LGUI, KC_ESC,
        KC_RCTL),

    /*
     *Shifted
     * .-----------------------------------------------------------------------------------------------------------------------------.---------
     * | Esc    | 1   °  | 2   §  | 3   ℓ  | 4   »  | 5   «  |        |        |
     *6  $   | 7    € | 8   „  | 9     “| 0    ” | ß   ẞ  | Back   |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|--------+
     * |  Tab   | Q      | W      | E      | R      | T      |        |        |
     *Z      | U      | I      | O      | P      | ü      | ̂       |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|--------+
     * |  Mod3  | A      | S      | D      | F      | G      |        |        |
     *H      | J      | K      | L      | Ö      | ä      | ̀       |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|--------+
     * | LShift | Y      | X      | C      | V      | B      |        |        |
     *N      | M      | , ;    | . :    | - —    | RShift |  ́      |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|--------+
     * | Ctrl   |  Super |  LAlt  | Mod3   |  Mod4  | Space  |        |        |
     *Enter  | Mod3   | Mod4   | Super  | Esc    |  Mod7  | RCtrl  |
     * '-----------------------------------------------------------------------------------------------------------------------------'---------
     */
    [_2] =
        LAYOUT_ortho_5x15(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS,
                          KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                          KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_NO, KC_NO,
                          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO),

    /* Symbol
     * .--------------------------------------------------------------------------------------------------------------------------------------.
     * |        |        |        |        |        |        |        |        |
     * |        |        |        |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        | …      | _      | [      | ]      | ^      |        |        |
     * !      | <      | >      | =      | &      | ſ       | ̷      |
     * |--------+--------+--------+--------+--------+--------+--------+-----------------+--------+--------+--------+--------+--------+--------|
     * |        | \      | /      | {      | }      | *      |        |        |
     * ?      | (      | )      | -      | :      |  @     |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        | #      | $      | |      | ~      | `      |        |        |
     * +      | %      | "      | '      | ;      |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        |        |        |        |        |        |        |        |
     * |        |        |        |        |        |        |
     * '--------------------------------------------------------------------------------------------------------------------------------------'
     *
     */
    [_3] = LAYOUT_ortho_5x15(
        KC_ESC, UC(L'¹'), UC(L'²'), UC(L'³'), UC(L'›'), UC(L'‹'), KC_TRNS,
        KC_TRNS, KC_BSPC, UC(L'¢'), KC_JYEN, UC(L'‚'), UC(L'‘'), KC_NO, KC_NO,
        KC_TAB, UC(L'…'), KC_UNDS, KC_LBRC, KC_RBRC, KC_CIRC, KC_TRNS, KC_TRNS,
        KC_INT1, KC_EXLM, KC_LT, KC_GT, KC_EQL, KC_AMPR, UC(L'ſ'), TG(_3),
        KC_BSLASH, KC_SLASH, KC_LCBR, KC_RCBR, DE_G, KC_TRNS, KC_TRNS, KC_NO,
        KC_QUES, KC_LPRN, KC_RPRN, KC_MINUS, KC_COLN, KC_AT, KC_LSFT, KC_HASH,
        KC_DLR, KC_PIPE, KC_TILD, KC_GRAVE, KC_TRNS, KC_TRNS, KC_NO, KC_PLUS,
        KC_PERC, KC_DQT, KC_QUOT, KC_SCLN, KC_RSFT, KC_LCTL, KC_LGUI, KC_LALT,
        TG(_3), MO(_6), KC_SPC, KC_TRNS, KC_TRNS, MO(_7), KC_ENTER, TG(_3),
        MO(_6), KC_LGUI, KC_ESC, KC_RCTL),

    /* NavNum

     * .--------------------------------------------------------------------------------------------------------------------------------------.
     * |        |        |        |        |        |        |        |        |
     |        |   /    |  *     |   -    |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        | HOME   | DELETE |  UP    | FDELETE|   END  |        |        |
     !!   |    7   |    8   |   9    |   +    |   —    |        |
     * |--------+--------+--------+--------+--------+--------+--------+-----------------+--------+--------+--------+--------+--------+--------|
     * |        | PGUP   |  LEFT  | DOWN   |  RIGHT | PGDOWN |        |        |
     ??   |    4   |    5   |   6    |   ,    |   .    |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        | ESC    |  TAB   | INSERT | ENTER  |  UNDO  |        |        |
     :     |    1   |    2   |   3    |  ;     |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        |        |        |        |        |        |        |        |
     ENTER |   0    |        |        |        |        |        |
     * '--------------------------------------------------------------------------------------------------------------------------------------'
     */
    [_4] = LAYOUT_ortho_5x15(
        KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO,
        KC_NO, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_HOME, KC_BSPC, KC_UP, KC_DELETE, KC_END, KC_TRNS,
        KC_TRNS, KC_NO, KC_NO, KC_7, KC_8, KC_9, KC_NO, KC_MINS, KC_NO, KC_PGUP,
        KC_LEFT, KC_DOWN, KC_RIGHT, KC_PGDOWN, KC_TRNS, KC_TRNS, KC_NO, KC_NO,
        KC_4, KC_5, KC_6, KC_MINS, KC_NO, KC_LSFT, KC_ESC, KC_PASTE, KC_TAB,
        KC_UNDO, KC_AGAIN, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_1, KC_2, KC_3,
        KC_TRNS, KC_RSFT, KC_LCTL, KC_LGUI, KC_NO, KC_NO, KC_NO, LCTL(KC_SPC),
        KC_TRNS, KC_TRNS, KC_NO, LALT(KC_ENTER), KC_0, KC_COMM, KC_DOT, KC_NO,
        KC_RCTL),
    /* Greek

     * .--------------------------------------------------------------------------------------------------------------------------------------.
     * |        |        |        |        |        |        |        |        |
    |        |        |        |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        |        |        |        |        |        |        |        |
    |        |        |        |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+-----------------+--------+--------+--------+--------+--------+--------|
     * |        |        |        |        |        |        |        |        |
    |        |        |        |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        |        |        |        |        |        |        |        |
    |        |        |        |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        |        |        |        |        |        |        |        |
    |        |        |        |        |        |        |
     * '--------------------------------------------------------------------------------------------------------------------------------------'
    [_5] = LAYOUT_ortho_5x15(
        KC_NO        ,KC_NO        ,KC_NO        ,KC_NO        ,KC_NO ,KC_NO
    ,KC_TRNS      ,KC_TRNS      ,KC_NO        ,KC_NO        ,KC_NO        ,KC_NO
    ,KC_NO        ,KC_NO        ,KC_NO        , KC_NO        ,KC_NO ,KC_NO
    ,KC_NO        ,KC_NO        ,KC_NO        ,KC_TRNS      ,KC_TRNS      ,KC_NO
    ,KC_NO        ,KC_NO        ,KC_NO        ,KC_NO        ,KC_NO        ,KC_NO
    , KC_NO        ,KC_NO        ,KC_NO        ,KC_NO        ,KC_NO ,KC_NO
    ,KC_TRNS      ,KC_TRNS      ,KC_NO        ,KC_NO        ,KC_NO        ,KC_NO
    ,KC_NO        ,KC_NO        ,KC_NO        , KC_NO        ,KC_NO ,KC_NO
    ,KC_NO        ,KC_NO        ,KC_NO        ,KC_TRNS      ,KC_TRNS      ,KC_NO
    ,KC_NO        ,KC_NO        ,KC_NO        ,KC_NO        ,KC_NO        ,KC_NO
    , KC_NO        ,KC_NO        ,KC_NO        ,KC_NO        ,KC_NO ,KC_NO
    ,KC_TRNS      ,KC_TRNS      ,KC_NO        ,KC_NO        ,KC_NO        ,KC_NO
    ,KC_NO        ,KC_NO        ,KC_NO
    ),
     */

    /* Math

     * .--------------------------------------------------------------------------------------------------------------------------------------.
     * |        |        |        |        |        |        |        |        |
    |        |        |        |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        |        |        |        |        |        |        |        |
    |        |        |        |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+-----------------+--------+--------+--------+--------+--------+--------|
     * |        |        |        |        |        |        |        |        |
    |        |        |        |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        |        |        |        |        |        |        |        |
    |        |        |        |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        |        |        |        |        |        |        |        |
    |        |        |        |        |        |        |
     * '--------------------------------------------------------------------------------------------------------------------------------------'
    [_6] = LAYOUT_ortho_5x15(
        KC_NO        ,KC_NO        ,KC_NO        ,KC_NO        ,KC_NO ,KC_NO
    ,KC_TRNS      ,KC_TRNS      ,KC_NO        ,KC_NO        ,KC_NO        ,KC_NO
    ,KC_NO        ,KC_NO        ,KC_NO        , KC_NO        ,KC_NO ,KC_NO
    ,KC_NO        ,KC_NO        ,KC_NO        ,KC_TRNS      ,KC_TRNS      ,KC_NO
    ,KC_NO        ,KC_NO        ,KC_NO        ,KC_NO        ,KC_NO        ,KC_NO
    , KC_NO        ,KC_NO        ,KC_NO        ,KC_NO        ,KC_NO ,KC_NO
    ,KC_TRNS      ,KC_TRNS      ,KC_NO        ,KC_NO        ,KC_NO        ,KC_NO
    ,KC_NO        ,KC_NO        ,KC_NO        , KC_NO        ,KC_NO ,KC_NO
    ,KC_NO        ,KC_NO        ,KC_NO        ,KC_TRNS      ,KC_TRNS      ,KC_NO
    ,KC_NO        ,KC_NO        ,KC_NO        ,KC_NO        ,KC_NO        ,KC_NO
    , KC_NO        ,KC_NO        ,KC_NO        ,KC_NO        ,KC_NO ,KC_NO
    ,KC_TRNS      ,KC_TRNS      ,KC_NO        ,KC_NO        ,KC_NO        ,KC_NO
    ,KC_NO        ,KC_NO        ,KC_NO
    ),
     */

    /* Mouse-Layer

     * .--------------------------------------------------------------------------------------------------------------------------------------.
     * |        |        |        |        |        |        |        |        |
     |        |        |        |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        |        |        | MouseU |        | WheelU |        |        |
     | Accel0 | Accel1 | Accel2 |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------+--------+--------+--------+--------|
     * |        |        | MouseL | MouseD | MouseR | WheelD |        |        |
     | Btn1   | Btn3   | Btn2   |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        |        |        |        |        |        |        |        |
     |        |        |        |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        |        |        |        |        |        | TG(_7) | TG(_7) |
     |        |        |        |        |        |        |
     * '--------------------------------------------------------------------------------------------------------------------------------------'
     */
    [_7] =
        LAYOUT_ortho_5x15(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS,
                          KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_NO, KC_NO, KC_NO, KC_MS_U, KC_NO, KC_WH_U,
                          KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_ACL0, KC_ACL1,
                          KC_ACL2, KC_NO, KC_NO, KC_NO, KC_NO, KC_MS_L, KC_MS_D,
                          KC_MS_R, KC_WH_D, KC_TRNS, KC_TRNS, KC_NO, KC_NO,
                          KC_BTN1, KC_BTN2, KC_BTN3, KC_NO, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_NO,
                          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, TG(_7), TG(_7),
                          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO),

    /* FN and Setting Layer

     * .--------------------------------------------------------------------------------------------------------------------------------------.
     * | Reset  |        |        |        |        |        |        |        |
     |        |        |        |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        | F1     | F2     | F3     | F4     |        |        |        |
     |        |        |        |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+-----------------+--------+--------+--------+--------+--------+--------|
     * |        | F5     | F6     | F7     | F8     |        |        |        |
     | QWERTZ?| QWERTY?|        |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        | F9     | F10    | F11    | F12    |        |        |        |
     |        |        |        |        |        |        |
     * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
     * |        |        |        |        |        |        |        |        |
     |        |        |        |        |        |        |
     * '--------------------------------------------------------------------------------------------------------------------------------------'
     */
    [_9] =
        LAYOUT_ortho_5x15(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS,
                          KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                          KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_NO, KC_NO, TG(_8), KC_TRNS, TG(_8), KC_NO,
                          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO),
    [_8] =
        LAYOUT_ortho_5x15(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS,
                          KC_TRNS, RESET, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_NO, KC_F1, KC_F2, KC_F3, KC_F4, KC_NO,
                          KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_NO, KC_NO, KC_F5, KC_F6, KC_F7, KC_F8,
                          KC_NO, KC_TRNS, KC_TRNS, KC_NO, KC_NO, QWERTZ, CUSTOM,
                          KC_NO, KC_NO, KC_NO, KC_LSFT, KC_F9, KC_F10, KC_F11,
                          KC_F12, KC_NO, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_LALT,
                          KC_NO, KC_NO, KC_NO, TG(_8), KC_TRNS, TG(_8), KC_NO,
                          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO),

    [_vim] = LAYOUT_ortho_5x15(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_U,
        KC_Q, KC_SLSH, KC_I, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS),

};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  /*case QWERTZ:
      set_single_persistent_default_layer(_DE);
      break;
  case CUSTOM:
      set_single_persistent_default_layer(_1);
      break;
      */
  case DELWORD:
    if (record->event.pressed) {
      SEND_STRING(SS_LCTL(SS_TAP(X_BSPC)));
    }
    break;
  }
  return true;
}
/*
**
bool process_record_user( uint16_t keycode, keyrecord_t *record ) {
    switch (keycode){
        case KC_RSFT:
        case KC_LSFT:
            if (record->event.pressed && shifted){
                SEND_STRING(SS_TAP(X_LCAP));
                return true;
            }
            shifted = record->event.pressed;
            break;
    }
    return true;
}


bool key(uint16_t k1, uint16_t k2){
    if (recird)


}
**
 */

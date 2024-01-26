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
/* #include "features/achordion.h" */

enum layer_names { _DE, _1, _4, _5, _6, _7, _8, _9 };
int shifted = 0;
bool key(uint16_t k1, uint16_t k2);

enum my_keycodes { QWERTZ, CUSTOM, DELWORD };

enum { TD_THUMBMOD_L, TD_THUMBMOD_R };

static bool thumbmod_cleanup_necessary = false;

void finished_thumbmod(tap_dance_state_t *state, void *user_data) {
  thumbmod_cleanup_necessary = state->pressed;
  switch (state->count) {
    case 1:
      if (state->pressed)
	register_code(KC_RCTL);
      else
	add_oneshot_mods(MOD_RCTL);
      break;
    case 2:
      if (state->pressed)
	register_code(KC_LALT);
      else
	add_oneshot_mods(MOD_LALT);
      break;
    case 3:
      if (state->pressed) {
	register_code(KC_LALT);
	register_code(KC_RCTL);
      }
      else {
	add_oneshot_mods(MOD_LALT);
	add_oneshot_mods(MOD_RCTL);
      }
      break;
  }
}


void reset_thumbmod(tap_dance_state_t *state, void *user_data) {
  if (thumbmod_cleanup_necessary)
    switch (state->count) {
      case 1:
	unregister_code(KC_RCTL);
	break;
      case 2:
	unregister_code(KC_LALT);
	break;
      case 3:
	unregister_code(KC_LALT);
	unregister_code(KC_RCTL);
	break;
    }
}

tap_dance_action_t tap_dance_actions[] = {
  // Tap once for LCTL, twice for LALT
  [TD_THUMBMOD_L] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, finished_thumbmod, reset_thumbmod),
  /* [TD_THUMBMOD_L] = ACTION_TAP_DANCE_FN(thumbdance), */
  [TD_THUMBMOD_R] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, finished_thumbmod, reset_thumbmod)
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    /* case TD_THUMBMOD_L: */
    /* case TD_THUMBMOD_R: */
    /*   return 150; */
    default:
      return 200;
  }
}

uint16_t control_pressed = 0;


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* standard DE Layout
     * Composing keys: KC_RBRC, KC_GRV, KC_EQL
     */
    [_DE] = LAYOUT_ortho_5x15(
                              KC_ESC , KC_1,    KC_2,    KC_3,    KC_4,            KC_5,     KC_INS,            TG(_7),   KC_PSCR,           KC_6,   KC_7,           KC_8,    KC_9,    KC_0,    KC_MINS,
                              KC_TAB , KC_Q,    KC_W,    KC_E,    KC_R,            KC_T,     KC_VOLD,           KC_MUTE,  KC_VOLU,           KC_Y,   KC_U,           KC_I,    KC_O,    KC_P,    KC_LBRC,
                              KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,            KC_G,     C(KC_F3),          C(KC_F1), C(KC_BSPC),        KC_H,   KC_J,           KC_K,    KC_L,    KC_SCLN, KC_QUOT,
                              KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,            KC_B,     KC_COPY,           KC_PSTE,  KC_NO,             KC_N,   KC_M,           KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                              KC_LCTL, KC_LGUI, KC_LALT, KC_CAPS, LT(_4, KC_MINS), KC_SPC,   KC_LCTL,           KC_LALT,  KC_RCTL,           KC_ENT, LT(_4, KC_0),   KC_NUHS, KC_LGUI, KC_RBRC, OSL(_9)),

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
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_PGUP, KC_BSPC, KC_UP, KC_DELETE, KC_PGDN, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_7, KC_8, KC_9, KC_NO, KC_MINS,
        KC_NO, KC_HOME, KC_LEFT, KC_DOWN, KC_RIGHT, KC_END, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_4, KC_5, KC_6, KC_MINS, KC_NO,
        KC_LSFT, KC_ESC, KC_PASTE, KC_TAB, KC_UNDO, KC_AGAIN, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_1, KC_2, KC_3, KC_TRNS, KC_RSFT,
        KC_LCTL, KC_LGUI, KC_NO, KC_NO, KC_NO, LCTL(KC_SPC), KC_TRNS, KC_TRNS, KC_NO, LALT(KC_ENTER), KC_0, KC_COMM, KC_DOT, KC_NO, KC_RCTL),
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
    [_9] = LAYOUT_ortho_5x15(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS,
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
    [_8] = LAYOUT_ortho_5x15(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS,
                          KC_TRNS, QK_BOOT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_NO, KC_F1, KC_F2, KC_F3, KC_F4, KC_NO,
                          KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_NO, KC_NO, KC_F5, KC_F6, KC_F7, KC_F8,
                          KC_NO, KC_TRNS, KC_TRNS, KC_NO, KC_NO, QWERTZ, CUSTOM,
                          KC_NO, KC_NO, KC_NO, KC_LSFT, KC_F9, KC_F10, KC_F11,
                          KC_F12, KC_NO, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_LALT,
                          KC_NO, KC_NO, KC_NO, TG(_8), KC_TRNS, TG(_8), KC_NO,
                          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO)
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  /* if (!process_achordion(keycode, record)) { */
  /*   return false; */
  /* } */
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
    /* case TD(TD_THUMBMOD_R): */
    /* case TD(TD_THUMBMOD_L): */
    /*   if (record->event.pressed) { */
    /*     if (record->event.pressed && control_pressed++ > 0) { */
    /*       clear_oneshot_mods(); */
    /*       tap_code16(C(KC_F3)); */
    /*       return false; */
    /*     } */
    /*   } else { */
    /*     control_pressed--; */
    /*   } */
    /*   break; */
  }
  return true;
}

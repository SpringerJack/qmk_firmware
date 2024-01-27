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

enum layer_names { _DE, _CMOD, _LMOD, _RMOD, _4, _7, _8, _9 };
int shifted = 0;
bool key(uint16_t k1, uint16_t k2);

/* enum my_keycodes { }; */

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
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
                              KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,            KC_B,     KC_COPY,           KC_PSTE,  TG(_7),            KC_N,   KC_M,           KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                              KC_NO,   KC_NO,   KC_NO,   KC_CAPS, MO(_4),          KC_SPC,   OSL(_LMOD),        KC_NO,    OSL(_RMOD),        KC_ENT, MO(_4),         KC_NUHS, KC_NO,   KC_RBRC, OSL(_9)),

    [_CMOD] = LAYOUT_ortho_5x15(
                              KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                              KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                              KC_TRNS, KC_TRNS, KC_LGUI, KC_LALT, KC_LCTL, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS,   KC_TRNS, KC_LCTL, KC_LALT, KC_LGUI, KC_TRNS, KC_TRNS,
                              KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                              KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, MO(_LMOD), KC_TRNS, MO(_RMOD), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

    [_RMOD] = LAYOUT_ortho_5x15(
                              KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                              KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                              KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_LCTL, KC_LALT, KC_LGUI, KC_TRNS, KC_TRNS,
                              KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                              KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

    [_LMOD] = LAYOUT_ortho_5x15(
                              KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                              KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                              KC_TRNS, KC_TRNS, KC_LGUI, KC_LALT, KC_LCTL, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                              KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                              KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

    [_4] = LAYOUT_ortho_5x15(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_PGUP, KC_BSPC, KC_UP, KC_DELETE, KC_PGDN, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_7, KC_8, KC_9, KC_NO, KC_MINS,
        KC_NO, KC_HOME, KC_LEFT, KC_DOWN, KC_RIGHT, KC_END, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_4, KC_5, KC_6, KC_MINS, KC_NO,
        KC_LSFT, KC_ESC, KC_PASTE, KC_TAB, KC_UNDO, KC_AGAIN, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_1, KC_2, KC_3, KC_TRNS, KC_RSFT,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

    /* Below is infrequently used */
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
                          KC_NO, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_NO, KC_NO, KC_LSFT, KC_F9, KC_F10, KC_F11,
                          KC_F12, KC_NO, KC_TRNS, KC_TRNS, KC_NO, KC_NO, KC_NO,
                          KC_NO, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_LALT,
                          KC_NO, KC_NO, KC_NO, TG(_8), KC_TRNS, TG(_8), KC_NO,
                          KC_NO, KC_NO, KC_NO, KC_NO, KC_NO)
};






#include QMK_KEYBOARD_H
#include "wait.h"
#include "tapdance.h"

/* generic code */
extern bool    is_drag_scroll;  
int16_t        dsAccX          = 0;
int16_t        dsAccY          = 0;
const uint16_t dsThreshhold    = 5;

static td_tap_t xtap_state = {.is_press_action = 1, .state = TD_NONE};

/* Tap Dance for Button 3 == Upper Thumb */
void x_observe_btn3(qk_tap_dance_state_t *state, void *user_data) {
  if (xtap_state.state == TD_NONE) {
    xtap_state.state = TD_SINGLE_HOLD;
    register_custom_keycode(DRAG_SCROLL, 2, 0);
  }
}

void x_finished_btn3(qk_tap_dance_state_t *state, void *user_data) {
  xtap_state.state = get_dance(state);
  switch (xtap_state.state) {
  case TD_SINGLE_TAP:
    unregister_custom_keycode(DRAG_SCROLL, 2, 0);
    register_code(KC_BTN3);
    break;
  case TD_SINGLE_HOLD:
    break;
  default:
    unregister_custom_keycode(DRAG_SCROLL, 2, 0);
    return;
  }
}

void x_reset_btn3(qk_tap_dance_state_t *state, void *user_data) {
  switch (xtap_state.state) {
  case TD_SINGLE_TAP:
    unregister_code(KC_BTN3);
    break;
  case TD_SINGLE_HOLD:
    unregister_custom_keycode(DRAG_SCROLL, 2, 0);
    break;
  default:
  }
  xtap_state.state = TD_NONE;
}
/* End Tap Dance for Button 3 */

/* Tap Dance Keys */
qk_tap_dance_action_t tap_dance_actions[] = {
    [X_UPPER_THUMB] = ACTION_TAP_DANCE_FN_ADVANCED(
        x_observe_btn3, x_finished_btn3, x_reset_btn3)};
/* End Tap Dance Config */

/* Main layout declaration */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(/* Base */
                 KC_BTN1, KC_BTN3, TD(X_UPPER_THUMB), KC_BTN2, LT(1, KC_BTN5)),
    [1] = LAYOUT(_______, _______, _______, _______, _______)};

/* Helper functions for higher level firmware */
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  case TD(X_UPPER_THUMB):
    return 150;
  default:
    return 200;
  }
}

int16_t sign(int16_t n) {
  return (int16_t) ((n > 0) - (n < 0));
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
  if (is_drag_scroll) {
    dsAccX += mouse_report.h;
    dsAccY += mouse_report.v;

    if (abs(dsAccX) > dsThreshhold) {
      mouse_report.h = sign(dsAccX);
      dsAccX %= dsThreshhold;
    } else {
      mouse_report.h = 0;
    }
      
    if (abs(dsAccY) > dsThreshhold) {
      mouse_report.v = sign(dsAccY);
      dsAccY %= dsThreshhold;
    } else {
      mouse_report.v = 0;
    }
  } 

  return mouse_report;
}


bool encoder_update_user(uint8_t index, bool clockwise) {
  return false;
}

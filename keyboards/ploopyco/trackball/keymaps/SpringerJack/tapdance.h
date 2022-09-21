/* Tap Dance Config */
typedef enum {
  TD_NONE,
  TD_UNKNOWN,
  TD_SINGLE_TAP,
  TD_TAP_HOLD,
  TD_DOUBLE_TAP,
  TD_SINGLE_HOLD,
} td_state_t;

typedef struct {
  bool is_press_action;
  td_state_t state;
} td_tap_t;

enum { X_UPPER_THUMB };


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

/* Decide which dance has been executed */
td_state_t get_dance(qk_tap_dance_state_t *state) {
  switch (state->count) {
    case 1:
      return !state->pressed ? TD_SINGLE_TAP : TD_SINGLE_HOLD;
    case 2:
      return !state->pressed ? TD_DOUBLE_TAP : TD_TAP_HOLD;
    default:
      return TD_UNKNOWN;
  }
}
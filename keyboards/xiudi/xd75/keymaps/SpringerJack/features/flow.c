/* Copyright 2022 @daliusd
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
#include "flow.h"

extern const uint16_t flow_config[FLOW_COUNT][2];
extern const uint16_t flow_layers_config[FLOW_LAYERS_COUNT][2];

// Represents the states a flow key can be in
typedef enum {
  flow_up_unqueued,
  flow_up_queued,
  flow_up_queued_used,
  flow_down_unused,
  flow_down_used,
} flow_state_t;

#ifdef FLOW_ONESHOT_TERM
const int g_flow_oneshot_term = FLOW_ONESHOT_TERM;
#else
const int g_flow_oneshot_term = 500;
#endif

#ifdef FLOW_ONESHOT_WAIT_TERM
const int g_flow_oneshot_wait_term = FLOW_ONESHOT_WAIT_TERM;
#else
const int g_flow_oneshot_wait_term = 500;
#endif

flow_state_t flow_layers_state[FLOW_LAYERS_COUNT] = {
  [0 ... FLOW_LAYERS_COUNT - 1] = flow_up_unqueued
};
bool flow_layer_timeout_timers_active[FLOW_LAYERS_COUNT] = { [0 ... FLOW_LAYERS_COUNT - 1] = false };
uint16_t flow_layer_timeout_timers_value[FLOW_LAYERS_COUNT] = { [0 ... FLOW_LAYERS_COUNT - 1] = 0 };
uint16_t flow_layer_timeout_wait_timers_value[FLOW_LAYERS_COUNT] = { [0 ... FLOW_LAYERS_COUNT - 1] = 0 };

bool update_flow(
                 uint16_t keycode,
                 bool pressed,
                 keypos_t key_position
                 ) {
  uint8_t key_layer = read_source_layers_cache(key_position);
  bool pass = true;

  for (int i = 0; i < FLOW_LAYERS_COUNT; i++) {
    uint16_t trigger = flow_layers_config[i][0];
    uint16_t layer = flow_layers_config[i][1];

    if (keycode == trigger) {
      if (pressed) {
        // Trigger keydown
        if (flow_layers_state[i] == flow_up_unqueued) {
          layer_on(layer);
          /* change_pressed_status(trigger, true); */
        }
        flow_layer_timeout_wait_timers_value[i] = timer_read();
        flow_layers_state[i] = flow_down_unused;
        pass = false;
      } else {
        // Trigger keyup
        switch (flow_layers_state[i]) {
          case flow_down_unused:
            if (timer_elapsed(flow_layer_timeout_wait_timers_value[i]) > g_flow_oneshot_wait_term) {
              flow_layers_state[i] = flow_up_unqueued;
              layer_off(layer);
              /* change_pressed_status(trigger, false); */
              pass = false;
            } else {
              // If we didn't use the layer while trigger was held, queue it.
              flow_layers_state[i] = flow_up_queued;
              flow_layer_timeout_timers_active[i] = true;
              flow_layer_timeout_timers_value[i] = timer_read();
              pass = false;
              /* change_pressed_status(trigger, true); */
            }
            break;
          case flow_down_used:
            // If we did use the layer while trigger was held, turn off it.
            flow_layers_state[i] = flow_up_unqueued;
            layer_off(layer);
            /* change_pressed_status(trigger, false); */
            pass = false;
            break;
          default:
            break;
        }
      }
    } else {
      if (pressed) {
        if (key_layer == layer) {
          // On non-ignored keyup, consider the oneshot used.
          switch (flow_layers_state[i]) {
            case flow_down_unused:
              flow_layers_state[i] = flow_down_used;
              break;
            case flow_up_queued:
              flow_layers_state[i] = flow_up_queued_used;
              flow_layer_timeout_timers_active[i] = false;
              break;
            case flow_up_queued_used:
              flow_layers_state[i] = flow_up_unqueued;
              layer_off(layer);
              /* change_pressed_status(trigger, false); */
              pass = false;
              break;
            default:
              break;
          }
        }
      } else {
        // Ignore key ups from other layers
        if (key_layer == layer) {
          // On non-ignored keyup, consider the oneshot used.
          switch (flow_layers_state[i]) {
            case flow_up_queued:
              flow_layers_state[i] = flow_up_unqueued;
              layer_off(layer);
              /* change_pressed_status(trigger, false); */
              break;
            case flow_up_queued_used:
              flow_layers_state[i] = flow_up_unqueued;
              layer_off(layer);
              /* change_pressed_status(trigger, false); */
              break;
            default:
              break;
          }
        }
      }
    }
  }

  return pass;
}

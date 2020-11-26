/* Copyright 2020 Coumbaya
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
#include QMK_KEYBOARD_H

#include "keymap_french.h"

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _LOWER,
    _RAISE,
    _ADJUST
};

// Custom keycodes for use in macros
enum custom_keycodes {
    QMKBEST,
    QMKURL
};

// LAYOUTS
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_ESC,           FR_A,   FR_Z,   KC_E,   KC_R,   KC_T,            KC_Y,  KC_U,   KC_I,   KC_O,   KC_P,   KC_BSPACE,
        LSFT_T(KC_TAB),   FR_Q,   KC_S,   KC_D,   KC_F,   KC_G,            KC_H,  KC_J,   KC_K,   KC_L,   FR_M,   KC_DELETE,
        KC_LCTRL,         FR_W,   KC_X,   KC_C,   KC_V,   KC_B,            KC_N,  KC_M,   KC_LEFT,   KC_UP,   KC_DOWN,   KC_RIGHT,
                KC_LGUI,   MO(_LOWER),   LALT_T(KC_SPC),             RSFT_T(KC_ENT),   MO(_RAISE),   MO(_ADJUST)
    ),   
	[_LOWER] = LAYOUT(
        FR_AMP, FR_EACU, FR_QUOT, FR_APOS, FR_LPRN, FR_MINS,               FR_EGRV, FR_UNDS, FR_CCED, FR_AGRV, FR_RPRN, FR_EQL,
        LSFT_T(KC_TAB),   KC_NO,   KC_NO, KC_BSLS, KC_TILD, KC_NUHS,               KC_LCBR, KC_QUOT, KC_BSLS, KC_LBRC, KC_RBRC, KC_DEL,
        KC_LCTRL, KC_NUBS,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                 KC_NO,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_NO, 
                                _______, _______, _______,               _______, _______, _______
    ),
	[_RAISE] = LAYOUT(
        FR_1,   FR_2,    FR_3,    FR_4,    FR_5,    FR_6,                 FR_7,    FR_8,    FR_9,    FR_0, FR_OVRR, FR_PLUS,
        LSFT_T(KC_TAB),  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_DEL,
        KC_LCTRL, KC_F11,  KC_F12, KC_COPY, KC_PSTE,   KC_NO,                KC_NO, KC_COMM, KC_HOME, KC_PGDN, KC_PGUP,  KC_END,
                                    _______, _______, _______,              _______, _______, _______
    ),
	[_ADJUST] = LAYOUT(
        _______, FR_TILD, FR_HASH, ALGR(KC_E), FR_LCBR, FR_LBRC,          FR_GRV, FR_BSLS, FR_CCIRC,   FR_AT, FR_RBRC, FR_RCBR,
        RGB_MOD, RGB_TOG, RGB_VAI,    RGB_SAI, RGB_HUI, FR_LCBR,         _______, FR_PERC,  HPT_TOG, HPT_BUZ, _______, FR_RCBR,
        RGB_RMOD, _______, RGB_VAD,    RGB_SAD, RGB_HUD, FR_LPRN,         FR_PIPE, FR_QUES,   FR_DOT, FR_SLSH, FR_SECT, _______,
                                        _______, _______, _______,         _______, _______, _______
    )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QMKBEST:
            if (record->event.pressed) {
                // when keycode QMKBEST is pressed
                SEND_STRING("QMK is the best thing ever!");
            } else {
                // when keycode QMKBEST is released
            }
            break;
        case QMKURL:
            if (record->event.pressed) {
                // when keycode QMKURL is pressed
                SEND_STRING("https://qmk.fm/\n");
            } else {
                // when keycode QMKURL is released
            }
            break;
    }
    return true;
}

#ifdef OLED_DRIVER_ENABLE
void oled_task_user(void) {
    // Host Keyboard Layer Status
    oled_write_ln_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case _BASE:
            oled_write_P(PSTR("AZRTY\n"), false);
            break;
        case _LOWER:
            oled_write_P(PSTR("LOWER\n"), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("RAISE\n"), false);
            break;
        case _ADJUST:
            oled_write_P(PSTR("ADJST\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undef"), false);
    }

    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);
}
#endif

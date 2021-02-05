/* Copyright 2020 plut0nium
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
#include <stdio.h>

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _QWERTY,
    _FN
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_QWERTY] = LAYOUT(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,              KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC, 
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,              KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_DEL,  
        MO(_FN), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,              KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_ENT,  
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_PGUP,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, 
        KC_LCTL, KC_LGUI, KC_LGUI, KC_LALT, KC_SPC,  KC_SPC,  KC_PGDN,  KC_SPC,  KC_SPC,  KC_RALT, KC_UP,   KC_RGHT, KC_RCTL  
    ),
    [_FN] = LAYOUT(
        KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,             KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_DEL,  
        RESET,   _______, KC_UP,   _______, _______, _______,           _______, _______, _______, KC_F11,  KC_F12,  _______, 
        _______, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______,           _______, _______, _______, _______, _______, _______,
        RGB_HUI, RGB_SAI, RGB_VAI, RGB_MOD, BL_DEC,  BL_TOGG, BL_INC,   _______, _______, _______, KC_MPLY, KC_VOLU, KC_MUTE,
        RGB_HUD, RGB_SAD, RGB_VAD, RGB_TOG, _______, _______, _______,  _______, _______, _______, KC_MPRV, KC_VOLD, KC_MNXT
    )
};

#ifdef OLED_DRIVER_ENABLE
static void render_logo(void) {
    static const char PROGMEM my_logo[] = {
        // '0x3E_logo_32x16', 32x16px
        0xff, 0x0f, 0x07, 0xf3, 0x73, 0xb3, 0x07, 0x0f, 0xff, 0x3f, 0x3f, 0xff, 0xff, 0x3f, 0x3f, 0xff, 
        0xe7, 0xe3, 0x33, 0x33, 0x03, 0x87, 0xff, 0xff, 0x03, 0x03, 0x33, 0x33, 0x33, 0xf3, 0xff, 0xff, 
        0x7f, 0x78, 0x70, 0x66, 0x67, 0x67, 0x70, 0x78, 0x7f, 0x67, 0x62, 0x78, 0x78, 0x62, 0x67, 0x7f, 
        0x73, 0x63, 0x67, 0x67, 0x60, 0x70, 0x7f, 0x7f, 0x60, 0x60, 0x67, 0x67, 0x67, 0x67, 0x7f, 0x7f
    };  
    oled_write_raw_P(my_logo, sizeof(my_logo));
}

#ifdef RGBLIGHT_ENABLE
extern rgblight_config_t rgblight_config;

void render_rgb_status(void) {
    oled_write_ln_P(PSTR("RGB"), false);
    if (!rgblight_config.enable) {
        oled_write_P(PSTR("  off\n\n\n"), false);
    }
    else {
        static char led_buf[24] = {0};
        snprintf(led_buf, sizeof(led_buf), "M:%3dH:%3dS:%3dV:%3d",
            (uint8_t)rgblight_config.mode,
            (uint8_t)rgblight_config.hue,
            (uint8_t)rgblight_config.sat,
            (uint8_t)rgblight_config.val);
        oled_write(led_buf, false);
    }
}
#endif

extern backlight_config_t backlight_config;

void render_backlight_status(void) {
    oled_write_ln_P(PSTR("BKL"), false);
    if (!backlight_config.enable) {
        oled_write_P(PSTR("  off"), false);
    }
    else {
        static char bkl_buf[8] = {0};
        snprintf(bkl_buf, sizeof(bkl_buf), "L:%3d",
            (uint8_t)backlight_config.level);
        oled_write(bkl_buf, false);
    }
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;  // flips the display 180 degrees if offhand
}

void oled_task_user(void) {
    render_logo();
    oled_set_cursor(0,2); // default logo is 16px high (2 lines)

    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer"), false);

    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            oled_write_P(PSTR("QWTY\n"), false);
            break;
        case _FN:
            oled_write_P(PSTR("FN\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undf"), false);
    }

    // Host Keyboard LED Status
    // uint8_t led_usb_state = host_keyboard_leds();
    // oled_write_P(PSTR("-----"), false);
    // oled_write_P(PSTR("Stats"), false);
    // oled_write_P(led_usb_state & (1<<USB_LED_NUM_LOCK) ? PSTR("num:*") : PSTR("num:."), false);
    // oled_write_P(led_usb_state & (1<<USB_LED_CAPS_LOCK) ? PSTR("cap:*") : PSTR("cap:."), false);
    // oled_write_P(led_usb_state & (1<<USB_LED_SCROLL_LOCK) ? PSTR("scr:*") : PSTR("scr:."), false);

    oled_write_P(PSTR("-----"), false);
    render_backlight_status();

    // Host Keyboard RGB status
#ifdef RGBLIGHT_ENABLE
    oled_write_P(PSTR("-----"), false);
    render_rgb_status();
#endif

}
#endif

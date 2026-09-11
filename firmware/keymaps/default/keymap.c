#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    
    [0] = LAYOUT(
        KC_1, KC_2, KC_3,
        KC_4, KC_5, KC_6,
        KC_7, KC_8, KC_9
    ),

    [1] = LAYOUT(
        UG_TOGG, UG_NEXT, UG_HUEU,
        UG_HUED, UG_SATU, UG_SATD,
        UG_VALU, UG_VALD, KC_TRNS
    )
};

#ifdef OLED_ENABLE
bool oled_task_user(void) {
    oled_write_ln("MY HACKPAD", false);

    if (get_higtest_layer(layer_state) == 1){
        oled_write_ln("RGB CONTROL", false);
    } else {
        oled_write_ln("READY", false);
    }

    return false;

}
#endif
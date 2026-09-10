#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        UG_TOGG, UG_NEXT, UG_PREV,
        UG_HUEU, UG_HUED, UG_SATU,
        UG_SATD, UG_VALU, UG_VALD
    )
};

#ifdef OLED_ENABLE
bool oled_task_user(void) {
    oled_write_ln("MY HACKPAD", false);
    oled_write_ln("RGB CONTROL", false);
    return false;
}
#endif
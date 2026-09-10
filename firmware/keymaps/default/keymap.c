#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        RGB_TOG, RGB_MOD, RGB_RMOD,
        RGB_HUI, RGB_HUD, RGB_SAI,
        RGB_SAD, RGB_VAI, RGB_VAD
    )
};

#ifdef OLED_ENABLE
bool oled_task_user(void) {
    oled_write_ln("MY HACKPAD", false);
    oled_write_ln("RGB CONTROL", false);
    return false;
}
#endif
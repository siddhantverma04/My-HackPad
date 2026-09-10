#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_1, KC_2, KC_3,
        KC_4, KC_5, KC_6,
        KC_7, KC_8, KC_9
    )
};

#ifdef OLED_ENABLE
bool oled_task_user(void) {
    oled_write_ln("MY HACKPAD", false);
    oled_write_ln("QMK READY!", false);

    return false;
}
#endif
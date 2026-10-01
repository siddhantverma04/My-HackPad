#include QMK_KEYBOARD_H
#include <stdlib.h>

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [0] = LAYOUT(
        C(KC_C),  C(KC_V),  KC_ESC,
        KC_L,     KC_UP,    KC_E,
        KC_LEFT,  KC_DOWN,  LT(1, KC_RGHT)
    ),

    [1] = LAYOUT(
        UG_TOGG, UG_NEXT, UG_HUEU,
        UG_HUED, UG_SATU, UG_SATD,
        UG_VALU, UG_VALD, KC_TRNS
    )
};

#ifdef OLED_ENABLE

// ---- Tuning ----
#define ENERGY_MAX        100   // top "excitement" level
#define ENERGY_PER_PRESS  25    // gained per key press
#define ENERGY_DECAY      5     // lost every 100 ms
#define SAD_AFTER_MS      2500  // no presses for this long -> sad
#define FRAME_SLOW_MS     450   // dance frame time at low energy
#define FRAME_FAST_MS     80    // dance frame time at max energy
#define SAD_FRAME_MS      700   // sad animation (tear) speed

// ---- Stick figure geometry (128x32 screen) ----
#define FIG_X       30
#define HEAD_R      4
#define HEAD_Y      5
#define SHOULDER_Y  12
#define HIP_Y       21
#define FOOT_DY     10

typedef struct {
    int8_t lean;        // body/head sway left (-) or right (+)
    int8_t bob;         // head up (-) or down (+)
    int8_t lhx, lhy;    // left hand, relative to shoulder
    int8_t rhx, rhy;    // right hand, relative to shoulder
    int8_t lfx, rfx;    // feet x offset from hip
} pose_t;

static const pose_t dance_poses[4] = {
    //lean bob   left hand    right hand    feet
    {  0, -1,   -9, -8,       9, -8,       -4,  4 },
    { -2,  0,  -11,  5,       9, -8,       -8,  4 },
    {  0, -1,   -9, -8,       9, -8,       -6,  6 },
    {  2,  0,   -9, -8,      11,  5,       -4,  8 },
};

static const pose_t sad_pose = { 1, 2, -4, 9, 4, 9, -3, 3 };

// ---- State ----
static uint16_t energy      = 0;
static uint32_t last_press  = 0;
static bool     has_pressed = false;
static uint32_t decay_timer = 0;
static uint32_t frame_timer = 0;
static uint8_t  frame       = 0;
static uint16_t last_sig    = 0xFFFF;

// ---- Drawing helpers ----
static void draw_line(int x0, int y0, int x1, int y1) {
    int dx  = abs(x1 - x0);
    int dy  = -abs(y1 - y0);
    int sx  = x0 < x1 ? 1 : -1;
    int sy  = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    while (true) {
        oled_write_pixel(x0, y0, true);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

static void draw_circle(int cx, int cy, int r) {
    int x = r, y = 0, d = 1 - r;
    while (x >= y) {
        oled_write_pixel(cx + x, cy + y, true);
        oled_write_pixel(cx - x, cy + y, true);
        oled_write_pixel(cx + x, cy - y, true);
        oled_write_pixel(cx - x, cy - y, true);
        oled_write_pixel(cx + y, cy + x, true);
        oled_write_pixel(cx - y, cy + x, true);
        oled_write_pixel(cx + y, cy - x, true);
        oled_write_pixel(cx - y, cy - x, true);
        y++;
        if (d < 0) {
            d += 2 * y + 1;
        } else {
            x--;
            d += 2 * (y - x) + 1;
        }
    }
}

static void draw_figure(const pose_t *p, bool happy, bool tear) {
    int hx   = FIG_X + p->lean;
    int hy   = HEAD_Y + p->bob;
    int hipx = FIG_X;

    // head + eyes
    draw_circle(hx, hy, HEAD_R);
    oled_write_pixel(hx - 1, hy - 1, true);
    oled_write_pixel(hx + 1, hy - 1, true);

    // mouth
    if (happy) {
        oled_write_pixel(hx - 2, hy + 1, true);
        oled_write_pixel(hx - 1, hy + 2, true);
        oled_write_pixel(hx,     hy + 2, true);
        oled_write_pixel(hx + 1, hy + 2, true);
        oled_write_pixel(hx + 2, hy + 1, true);
    } else {
        oled_write_pixel(hx - 2, hy + 2, true);
        oled_write_pixel(hx - 1, hy + 1, true);
        oled_write_pixel(hx,     hy + 1, true);
        oled_write_pixel(hx + 1, hy + 1, true);
        oled_write_pixel(hx + 2, hy + 2, true);
        // falling tear
        if (tear) {
            oled_write_pixel(hx + 5, hy + 3, true);
            oled_write_pixel(hx + 5, hy + 4, true);
        } else {
            oled_write_pixel(hx + 5, hy + 1, true);
            oled_write_pixel(hx + 5, hy + 2, true);
        }
    }

    // neck, torso, arms, legs
    draw_line(hx, hy + HEAD_R, hx, SHOULDER_Y);
    draw_line(hx, SHOULDER_Y, hipx, HIP_Y);
    draw_line(hx, SHOULDER_Y, hx + p->lhx, SHOULDER_Y + p->lhy);
    draw_line(hx, SHOULDER_Y, hx + p->rhx, SHOULDER_Y + p->rhy);
    draw_line(hipx, HIP_Y, hipx + p->lfx, HIP_Y + FOOT_DY);
    draw_line(hipx, HIP_Y, hipx + p->rfx, HIP_Y + FOOT_DY);
}

// ---- Count every key press as "energy" ----
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        energy += ENERGY_PER_PRESS;
        if (energy > ENERGY_MAX) energy = ENERGY_MAX;
        last_press  = timer_read32();
        has_pressed = true;
    }
    return true;
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_180;
}

bool oled_task_user(void) {
    // energy slowly drains away
    if (timer_elapsed32(decay_timer) >= 100) {
        decay_timer = timer_read32();
        energy = (energy > ENERGY_DECAY) ? energy - ENERGY_DECAY : 0;
    }

    bool happy = has_pressed && (timer_elapsed32(last_press) < SAD_AFTER_MS);

    // more energy = shorter frame time = faster dance
    uint32_t interval = happy
        ? FRAME_SLOW_MS - ((uint32_t)energy * (FRAME_SLOW_MS - FRAME_FAST_MS)) / ENERGY_MAX
        : SAD_FRAME_MS;

    if (timer_elapsed32(frame_timer) >= interval) {
        frame_timer = timer_read32();
        frame = (frame + 1) & 3;
    }

    uint8_t  layer = get_highest_layer(layer_state);
    uint8_t  f     = happy ? frame : (frame & 1);
    uint16_t sig   = f | (happy ? 0x10 : 0) | (layer == 1 ? 0x20 : 0);

    // only redraw when something actually changed
    if (sig == last_sig) return false;
    last_sig = sig;

    oled_clear();

    if (happy) {
        draw_figure(&dance_poses[f], true, false);
    } else {
        draw_figure(&sad_pose, false, (f & 1));
    }

    oled_set_cursor(9, 0);
    oled_write_P(PSTR("MY HACKPAD"), false);
    oled_set_cursor(9, 1);
    oled_write_P(happy ? PSTR("DANCING!") : PSTR("SAD..."), false);
    if (layer == 1) {
        oled_set_cursor(9, 2);
        oled_write_P(PSTR("RGB CONTROL"), false);
    }

    return false;
}

#endif
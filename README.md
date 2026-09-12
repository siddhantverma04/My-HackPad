# My HackPad

A custom 3×3 macropad designed from scratch using KiCad, Fusion 360, and QMK.

The HackPad uses a Seeed Studio XIAO RP2040, 9 mechanical switches,
4 SK6812MINI-E RGB LEDs, a 128×32 OLED display, and custom QMK firmware.

## Features

- 3×3 mechanical key layout
- 4 SK6812MINI-E RGB LEDs
- 0.91" 128×32 SSD1306 OLED
- Seeed Studio XIAO RP2040
- Custom PCB
- Custom 3D-printed case
- QMK firmware

## HackPad Render

<img width="350" height="328" alt="Screenshot 2026-09-09 213215" src="https://github.com/user-attachments/assets/6960187f-ca90-445e-aa31-81a7a1433506" />

## Schematic

<img width="645" height="393" alt="Screenshot 2026-09-11 185303" src="https://github.com/user-attachments/assets/b92275fb-302d-46d0-8671-124802f47fff" />

## PCB

<img width="303" height="358" alt="Screenshot 2026-09-11 185420" src="https://github.com/user-attachments/assets/919e4672-89c3-4016-9acb-c550d6a7b4eb" />

## Case

The case was designed to fit the custom PCB, switches, OLED, and XIAO RP2040. 

<img width="375" height="349" alt="Screenshot 2026-09-11 185621" src="https://github.com/user-attachments/assets/957942a4-0d08-4fa2-8eb5-18e0bf17f8b1" />

## Firmware

The HackPad runs QMK firmware on the XIAO RP2040.

Current normal keymap:

| 1 | 2 | 3 |
|---|---|---|
| 4 | 5 | 6 |
| 7 | 8 | 9 |

Holding key 9 opens an RGB-control layer for changing effects,
hue, saturation, and brightness.

## Bill of Materials

| Component | Amount |
|---|---:|
| Seeed Studio XIAO RP2040 | 1 |
| Cherry MX-compatible switches | 9 |
| 1N4148 diodes | 9 |
| SK6812MINI-E RGB LEDs | 4 |
| 0.91" 128×32 SSD1306 OLED | 1 |
| 1U keycaps | 9 |
| Custom PCB | 1 |
| 3D-printed top case / plate(I will print myself) | 1 |
| 3D-printed bottom case(I will print myself) | 1 |
| M3 screws | 4 |
| M3 heat-set inserts | 4 |

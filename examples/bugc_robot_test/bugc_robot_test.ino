/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 *
 *
 * @Hardwares: M5StickC/M5stickC-Plus/M5stickC-Plus2 + Hat BugC2
 * @Platform Version: Arduino M5Stack Board Manager v2.1.1
 * @Dependent Library:
 * M5GFX@^0.2.3: https://github.com/m5stack/M5GFX
 * M5Unified@^0.2.2: https://github.com/m5stack/M5Unified
 * M5HatBugC: https://github.com/m5stack/M5Hat-BugC
 */

#include <M5Unified.h>
#include "M5Hatbugc.h"
#include "img.h"

M5HatBugC bugc;

uint8_t bugc_motion_status = 0;
bool bugc_enable           = false;
uint32_t color1            = 0;
uint32_t color2            = 0;

void view_img(bugc_motion_t motion);  // Displays an image on the screen based on the motion type.
void bugc_motion(bugc_motion_t motion, uint32_t color1,
                 uint32_t color2);  // Handles motion behavior and sets colors for visual feedback.
uint32_t generateRandomUint32();    // Generates a random 32-bit unsigned integer.
void delay_ms(uint32_t ms);         //  Implements a non-blocking millisecond delay using millis().
void setup()
{
    M5.begin();
    M5.Display.begin();
    Serial.begin(115200);
    while (!bugc.begin(&Wire, BUGC_DEFAULT_I2C_ADDR, 0, 26, 400000U)) {
        Serial.println("Couldn't find BugC");
        delay(1000);
    }
    bugc.setAllMotorSpeed(0, 0, 0, 0);
    view_img(MOVE_STOP);
}

void loop()
{
    if (bugc_enable) {
        color1 = generateRandomUint32();
        color2 = generateRandomUint32();
        bugc_motion((bugc_motion_t)bugc_motion_status, color1, color2);
        bugc_motion_status++;
        bugc_motion_status = bugc_motion_status % 6;
    } else {
        view_img(MOVE_STOP);
        delay_ms(1000);
    }
}

void view_img(bugc_motion_t motion)
{
    float stickc_bat     = 0;
    float bugc_bat       = 0;
    uint16_t bat_voltage = bugc.getRawAdc12Bit();
    bugc_bat             = ((float)bat_voltage / 4095.0 * 3.3) * 2.960784;
    // Define a structure to store image configurations for each board
    struct ImageConfig {
        int width;                      // Width of the image
        int height;                     // Height of the image
        const uint16_t *image_data[7];  // Image data pointers for each motion type
    };

    // Predefine image configurations for different boards
    static const ImageConfig imageConfigs[] = {
        {80,
         160,
         {// Configuration for M5StickC
          image_data_forward_80_160, image_data_backward_80_160, image_data_left_80_160, image_data_right_80_160,
          image_data_right_rotation_80_160, image_data_left_rotation_80_160, image_data_stop_80_160}},
        {135,
         240,
         {// Configuration for M5StickCPlus
          image_data_forward_135_240, image_data_backward_135_240, image_data_left_135_240, image_data_right_135_240,
          image_data_right_rotation_135_240, image_data_left_rotation_135_240, image_data_stop_135_240}},
        {135,
         240,
         {// Configuration for M5StickCPlus2 (same as M5StickCPlus)
          image_data_forward_135_240, image_data_backward_135_240, image_data_left_135_240, image_data_right_135_240,
          image_data_right_rotation_135_240, image_data_left_rotation_135_240, image_data_stop_135_240}}};

    // Get the current board type
    m5::board_t board         = M5.getBoard();
    const ImageConfig *config = nullptr;

    // Select the corresponding image configuration based on the board type
    switch (board) {
        case m5::board_t::board_M5StickC:
            stickc_bat = M5.Power.Axp192.getBatteryVoltage();
            config     = &imageConfigs[0];
            break;
        case m5::board_t::board_M5StickCPlus:
            stickc_bat = M5.Power.Axp192.getBatteryVoltage();
            config     = &imageConfigs[1];
            break;
        case m5::board_t::board_M5StickCPlus2:
            stickc_bat = M5.Power.getBatteryVoltage() / 1000.0;
            config     = &imageConfigs[1];
            break;
        default:
            return;  // Unknown board type, exit the function
    }

    // Validate the motion type and configuration before displaying the image
    if (motion >= MOVE_FORWARD && motion <= MOVE_STOP && config) {
        // Display the image for the specified motion and board type
        M5.Display.pushImage(0, 0, config->width, config->height, config->image_data[motion]);
    }
    M5.Display.setCursor(1, 1);
    M5.Display.setTextSize(1);
    if (stickc_bat > 3.3) {
        M5.Display.setTextColor(BLUE);
    } else {
        M5.Display.setTextColor(RED);
    }
    M5.Display.printf("stac:%.1fV\r\n", stickc_bat);
    M5.Display.setCursor(1, 15);
    M5.Display.setTextSize(1);
    if (bugc_bat > 3.3) {
        M5.Display.setTextColor(BLUE);
    } else {
        M5.Display.setTextColor(RED);
    }
    M5.Display.printf("bugc:%.1fV\r\n", bugc_bat);
}

void bugc_motion(bugc_motion_t motion, uint32_t color1, uint32_t color2)
{
    bugc.setAllLedColor(color1, color2);
    bugc.move(motion, 50);
    view_img(motion);
    delay_ms(1000);
    bugc.move(MOVE_STOP, 50);
    view_img(MOVE_STOP);
    delay_ms(2000);
}

uint32_t generateRandomUint32()
{
    uint32_t highPart = random(0, 0xFFFF);  // Generate high 16 bits
    uint32_t lowPart  = random(0, 0xFFFF);  // Generate low 16 bits
    return (highPart << 16) | lowPart;      // Combine high and low parts into a 32-bit number
}
void delay_ms(uint32_t ms)
{
    unsigned long startMillis = millis();
    while (millis() - startMillis < ms) {
        M5.update();
        if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(500)) {  // BtnA pressed toggles BugC working state
            bugc_enable = bugc_enable ^ true;
        }
    }
}
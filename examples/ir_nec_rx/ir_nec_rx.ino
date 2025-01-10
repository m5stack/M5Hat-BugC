/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 *
 *
 * @Hardwares: M5StickC + Hat BugC2
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5HatBugC: https://github.com/m5stack/M5Hat-BugC
 * IRremote: https://github.com/Arduino-IRremote/Arduino-IRremote
 */

#include "M5HatBugC.h"
#include <IRremote.hpp>

M5HatBugC bugc;

void setup()
{
    Serial.begin(115200);
    while (!bugc.begin(&Wire, BUGC_DEFAULT_I2C_ADDR, 0, 26, 400000U)) {
        Serial.println("Couldn't find BugC");
        delay(1000);
    }
    bugc.setAllMotorSpeed(0, 0, 0, 0);
    bugc.setAllLedColor(0xff0000, 0x0000ff);

    // only BugC2 support
    IrReceiver.begin(BUGC2_IR_RX_PIN, ENABLE_LED_FEEDBACK);
}

void loop()
{
    /*
     * Check if received data is available and if yes, try to decode it.
     * Decoded result is in the IrReceiver.decodedIRData structure.
     *
     * E.g. command is in IrReceiver.decodedIRData.command
     * address is in command is in IrReceiver.decodedIRData.address
     * and up to 32 bit raw data in IrReceiver.decodedIRData.decodedRawData
     *
     * At 115200 baud, printing takes 40 ms for NEC protocol and 10 ms for NEC
     * repeat
     */
    if (IrReceiver.decode()) {
        Serial.println();
        /*
         * !!!Important!!! Enable receiving of the next value,
         * since receiving has stopped after the end of the current received
         * data packet.
         */
        IrReceiver.resume();

        /*
         * Finally check the received data and perform actions according to the
         * received address and commands
         */
        Serial.printf("address: %04X\r\n", IrReceiver.decodedIRData.address);
        Serial.printf("command: %04X\r\n", IrReceiver.decodedIRData.command);
    }

    /*
     * Your code here
     * For all users of the FastLed library, use this code for strip.show() to
     * improve receiving performance (which is still not 100%): if
     * (IrReceiver.isIdle()) { strip.show();
     * }
     */
}

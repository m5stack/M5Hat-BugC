#ifndef _M5_HAT_BUGC_H_
#define _M5_HAT_BUGC_H_

#include "Arduino.h"
#include "Wire.h"
#include "I2C_Class.h"

#define BUGC_DEFAULT_I2C_ADDR 0x38

#define BUGC_MOTOR_REG     0x00
#define BUGC_RGB_LED_REG   0x10
#define BUGC_ADC_8BIT_REG  0x20
#define BUGC_ADC_12BIT_REG 0x30
#define BUGC2_IR_RX_PIN    36

typedef enum {
    MOVE_FORWARD = 0,
    MOVE_BACKWARD,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_ROTATE,
    MOVE_REVERSE_ROTATE,
    MOVE_STOP,

} bugc_motion_t;

class M5HatBugC {
   private:
    uint8_t _addr;
    I2C_Class _i2c;

   public:
    /**
     * @brief
     *
     * @param wire
     * @param addr device i2c addr
     * @param sda sda pin number
     * @param scl scl pin number
     * @param freq  i2c speed
     * @return bool true=success / false=fail
     */
    bool begin(TwoWire* wire = &Wire, uint8_t addr = BUGC_DEFAULT_I2C_ADDR,
               uint8_t sda = 0, uint8_t scl = 26, long freq = 400000U);

    /**
     * @brief Set the Motor Speed object
     *
     * @param ch 0-3
     * @param speed -127~127
     * @return bool true=success / false=fail
     */
    bool setMotorSpeed(uint8_t ch, int8_t speed);

    /**
     * @brief Set the All Motor Speed object
     *
     * @param speed_0 channel0 speed -127~127
     * @param speed_1 channel1 speed -127~127
     * @param speed_2 channel2 speed -127~127
     * @param speed_3 channel3 speed -127~127
     * @return bool true=success / false=fail
     */
    bool setAllMotorSpeed(int8_t speed_0, int8_t speed_1, int8_t speed_2,
                          int8_t speed_3);

    /**
     * @brief Get the Motor Speed object
     *
     * @param ch 0-3
     * @return int8_t speed -127~127
     */
    int8_t getMotorSpeed(uint8_t ch);

    /**
     * @brief Set the Led Color object
     *
     * @param index 0-1 0=left / 1=right
     * @param color 3byte RGB
     * @return bool true=success / false=fail
     */
    bool setLedColor(uint32_t index, uint32_t color);

    /**
     * @brief Set the All Led Color object
     *
     * @param color_0_left left color 3byte RGB
     * @param color_1_right right color 3byte RGB
     * @return bool
     */
    bool setAllLedColor(uint32_t color_0_left, uint32_t color_1_right);

    /**
     * @brief Get the Raw Adc12 Bit object
     *
     * @return uint16_t 0-4095
     */
    uint16_t getRawAdc12Bit();

    /**
     * @brief Get the Raw Adc8 Bit object
     *
     * @return uint8_t 0-255
     */
    uint8_t getRawAdc8Bit();

   public:
    /**
     * @brief
     *
     * @param motion bugc_motion_t
     * @param speed 0-127
     * @return bool
     */
    bool move(bugc_motion_t motion, int8_t speed = 50);
};

#endif
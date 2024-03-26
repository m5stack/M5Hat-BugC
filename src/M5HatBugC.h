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
    bool begin(TwoWire* wire = &Wire, uint8_t addr = BUGC_DEFAULT_I2C_ADDR,
               uint8_t sda = 0, uint8_t scl = 26, long freq = 400000U);

    bool setMotorSpeed(uint8_t ch, int8_t speed);
    bool setAllMotorSpeed(int8_t speed_0, int8_t speed_1, int8_t speed_2,
                          int8_t speed_3);

    int8_t getMotorSpeed(uint8_t ch);

    bool setLedColor(uint32_t index, uint32_t color);
    bool setAllLedColor(uint32_t color_0_left, uint32_t color_1_right);

    uint16_t getRawAdc12Bit();
    uint8_t getRawAdc8Bit();

   public:
    bool move(bugc_motion_t motion, int8_t speed = 50);
};

#endif
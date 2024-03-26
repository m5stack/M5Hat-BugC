#include "M5HatBugC.h"

bool M5HatBugC::begin(TwoWire* wire, uint8_t addr, uint8_t sda, uint8_t scl,
                      long freq) {
    _i2c.begin(wire, sda, scl, freq);
    _addr = addr;
    return _i2c.exist(_addr);
}

// if speed > 0; clockwise rotation
bool M5HatBugC::setMotorSpeed(uint8_t ch, int8_t speed) {
    speed = (speed > 100) ? 100 : speed;
    speed = (speed < -100) ? -100 : speed;
    return _i2c.writeByte(_addr, ch, speed);
}

int8_t M5HatBugC::getMotorSpeed(uint8_t ch) {
    return _i2c.readByte(_addr, ch);
}

bool M5HatBugC::setAllMotorSpeed(int8_t speed_0, int8_t speed_1, int8_t speed_2,
                                 int8_t speed_3) {
    int8_t speed_out[4] = {speed_0, speed_1, speed_2, speed_3};
    for (uint8_t i = 0; i < 4; i++) {
        speed_out[i] = (speed_out[i] > 100) ? 100 : speed_out[i];
        speed_out[i] = (speed_out[i] < -100) ? -100 : speed_out[i];
    }
    return _i2c.writeBytes(_addr, BUGC_MOTOR_REG, (uint8_t*)speed_out, 4);
}

bool M5HatBugC::setLedColor(uint32_t index, uint32_t color) {
    uint8_t color_data[4];
    color_data[0] = index;
    color_data[1] = (color & 0xff0000) >> 16;
    color_data[2] = (color & 0x00ff00) >> 8;
    color_data[3] = (color & 0x0000ff);
    return _i2c.writeBytes(_addr, BUGC_RGB_LED_REG, color_data, 4);
}

bool M5HatBugC::setAllLedColor(uint32_t color_0_left, uint32_t color_1_right) {
    uint8_t color_data_left[4];
    color_data_left[0] = 0;
    color_data_left[1] = (color_0_left & 0xff0000) >> 16;
    color_data_left[2] = (color_0_left & 0x00ff00) >> 8;
    color_data_left[3] = (color_0_left & 0x0000ff);
    bool left = _i2c.writeBytes(_addr, BUGC_RGB_LED_REG, color_data_left, 4);
    delay(10);
    uint8_t color_data_right[4];
    color_data_right[0] = 1;
    color_data_right[1] = (color_1_right & 0xff0000) >> 16;
    color_data_right[2] = (color_1_right & 0x00ff00) >> 8;
    color_data_right[3] = (color_1_right & 0x0000ff);
    bool right = _i2c.writeBytes(_addr, BUGC_RGB_LED_REG, color_data_right, 4);
    return (left && right);
}

uint16_t M5HatBugC::getRawAdc12Bit(void) {
    uint8_t buf[2];
    _i2c.readBytes(_addr, BUGC_ADC_12BIT_REG, buf, 2);
    return buf[0] | (buf[1] << 8);
}

uint8_t M5HatBugC::getRawAdc8Bit(void) {
    return _i2c.readByte(_addr, BUGC_ADC_8BIT_REG);
}

bool M5HatBugC::move(bugc_motion_t motion, int8_t speed) {
    switch (motion) {
        case MOVE_FORWARD:
            return setAllMotorSpeed(speed, -speed, speed, -speed);
            break;
        case MOVE_BACKWARD:
            return setAllMotorSpeed(-speed, speed, -speed, speed);
            break;
        case MOVE_LEFT:
            return setAllMotorSpeed(-speed, -speed, speed, speed);
            break;
        case MOVE_RIGHT:
            return setAllMotorSpeed(speed, speed, -speed, -speed);
            break;
        case MOVE_ROTATE:
            return setAllMotorSpeed(speed, speed, speed, speed);
            break;
        case MOVE_REVERSE_ROTATE:
            return setAllMotorSpeed(-speed, -speed, -speed, -speed);
            break;
        case MOVE_STOP:
            return setAllMotorSpeed(0, 0, 0, 0);
            break;
    }
    return false;
}

#ifndef DFROBOT_MAX30102_H
#define DFROBOT_MAX30102_H

#include "hardware/i2c.h"
#include "pico/stdlib.h"

// Default I2C address for the DFRobot module
#define MAX30102_I2C_ADDR 0x57 // 

typedef struct {
    int spo2;
    int32_t heartbeat;
} max30102_data_t;

// Function prototypes
bool dfrobot_max30102_init(i2c_inst_t *i2c);
void dfrobot_max30102_start_collect(i2c_inst_t *i2c);
void dfrobot_max30102_end_collect(i2c_inst_t *i2c);
void dfrobot_max30102_get_data(i2c_inst_t *i2c, max30102_data_t *data);
float dfrobot_max30102_get_temperature(i2c_inst_t *i2c);

#endif // DFROBOT_MAX30102_H
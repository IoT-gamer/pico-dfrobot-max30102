#include "dfrobot_max30102.h"

// Internal helper for reading registers
static int16_t read_reg(i2c_inst_t *i2c, uint8_t reg_addr, uint8_t *data_buf, uint8_t len) {
    i2c_write_blocking(i2c, MAX30102_I2C_ADDR, &reg_addr, 1, true); // Keep master control
    int result = i2c_read_blocking(i2c, MAX30102_I2C_ADDR, data_buf, len, false);
    return result;
}

// Internal helper for writing registers
static void write_reg(i2c_inst_t *i2c, uint8_t reg_addr, const uint8_t *data_buf, uint8_t len) {
    uint8_t buffer[len + 1];
    buffer[0] = reg_addr;
    for (uint8_t i = 0; i < len; i++) {
        buffer[i + 1] = data_buf[i];
    }
    i2c_write_blocking(i2c, MAX30102_I2C_ADDR, buffer, len + 1, false);
}

bool dfrobot_max30102_init(i2c_inst_t *i2c) {
    uint8_t rbuf[2];
    read_reg(i2c, 0x04, rbuf, 2);
    // The device address register (0x04) should return 0x20.
    uint16_t dev_id = (rbuf[0] << 8) | rbuf[1];
    return (dev_id == 0x0020); 
}

void dfrobot_max30102_start_collect(i2c_inst_t *i2c) {
    // Write [0x00, 0x01] to register 0x20 to start collecting.
    uint8_t wbuf[2] = {0x00, 0x01};
    write_reg(i2c, 0x20, wbuf, 2);
}

void dfrobot_max30102_end_collect(i2c_inst_t *i2c) {
    // Write [0x00, 0x02] to register 0x20 to stop collecting.
    uint8_t wbuf[2] = {0x00, 0x02};
    write_reg(i2c, 0x20, wbuf, 2);
}

void dfrobot_max30102_get_data(i2c_inst_t *i2c, max30102_data_t *data) {
    uint8_t rbuf[8];
    // Read 8 bytes from register 0x0C.
    read_reg(i2c, 0x0C, rbuf, 8);
    
    // Byte 0 is SPO2.
    data->spo2 = rbuf[0];
    if (data->spo2 == 0) {
        data->spo2 = -1; // -1 indicates invalid/no data.
    }

    // Bytes 2, 3, 4, 5 make up the 32-bit heartbeat value.
    data->heartbeat = ((uint32_t)rbuf[2] << 24) | 
                      ((uint32_t)rbuf[3] << 16) | 
                      ((uint32_t)rbuf[4] << 8)  | 
                      ((uint32_t)rbuf[5]);
                      
    if (data->heartbeat == 0) {
        data->heartbeat = -1; // -1 indicates invalid/no data.
    }
}

float dfrobot_max30102_get_temperature(i2c_inst_t *i2c) {
    uint8_t temp_buf[2];
    // Read 2 bytes from register 0x14.
    read_reg(i2c, 0x14, temp_buf, 2);
    // Calculation: Byte 0 + (Byte 1 / 100.0).
    return (float)temp_buf[0] + ((float)temp_buf[1] / 100.0f);
}
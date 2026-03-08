# DFRobot MAX30102 C Library for Raspberry Pi Pico SDK

A lightweight C library for the Raspberry Pi Pico and Pico 2W to interface with the [DFRobot Gravity MAX30102 PPG Heart Rate and Oximeter Sensor](https://www.dfrobot.com/product-2529.html). 

This module features an integrated MCU that handles the complex PhotoPlethysmoGraphy (PPG) calculations natively. This library utilizes the Pico C SDK `hardware_i2c` to poll the pre-calculated heart rate and blood oxygen (SPO2) values, minimizing resource usage on the main controller.

## Features
* **FetchContent-Ready:** Easily integrate into existing Pico C SDK CMake projects.
* **Low Overhead:** Reads processed data directly from the module's integrated MCU.
* **Temperature Reading:** Supports polling the onboard sensor temperature.

## Hardware Connections

The module uses the standard I2C interface with a default address of `0x57`.

| Pico / Pico 2W Pin | DFRobot MAX30102 Pin |
| :--- | :--- |
| 3V3 (Out) | VCC |
| GND | GND |
| GP4 (I2C0 SDA) | SDA |
| GP5 (I2C0 SCL) | SCL |

> **Note:** The module updates its internal data every 4 seconds. Polling faster than this will simply return duplicate readings.

## Integration via CMake (FetchContent)

This library is structured to be easily consumed via CMake's `FetchContent`. In your main project's `CMakeLists.txt`:

```cmake
include(FetchContent)

FetchContent_Declare(
    dfrobot_max30102
    GIT_REPOSITORY https://github.com/IoT-gamer/pico-dfrobot-max30102.git
    GIT_TAG main # or specify a specific tag/commit
)
FetchContent_MakeAvailable(dfrobot_max30102)

# Link to your executable
target_link_libraries(your_executable PRIVATE 
    pico_stdlib 
    hardware_i2c 
    dfrobot_max30102
)
```

## Quick Start Example
[basic_reading.c](examples/basic_reading.c) demonstrates how to initialize the sensor and read heart rate, SPO2, and temperature values in a loop.

## API Reference
`bool dfrobot_max30102_init(i2c_inst_t *i2c)` Initializes the sensor and verifies the device ID. Returns true if successful.

`void dfrobot_max30102_start_collect(i2c_inst_t *i2c)` Commands the module's onboard MCU to begin collecting and calculating PPG data.

`void dfrobot_max30102_end_collect(i2c_inst_t *i2c)` Commands the module to stop data collection and enter a low-power state.

`void dfrobot_max30102_get_data(i2c_inst_t *i2c, max30102_data_t *data)` Populates the provided max30102_data_t struct with the latest SPO2 and Heartbeat values. Returns -1 for either value if the sensor is still measuring or if the reading is invalid.

`float dfrobot_max30102_get_temperature(i2c_inst_t *i2c)` Returns the current board temperature in Celsius.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments
* Thanks to DFRobot for creating the MAX30102 module and providing the Arduino repo [DFRobot_BloodOxygen_S](https://github.com/DFRobot/DFRobot_BloodOxygen_S) used as a reference for this library's functionality.
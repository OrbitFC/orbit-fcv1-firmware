#ifndef __LSM303_H__
#define __LSM303_H__
#include <stdbool.h>
#include <stm32h7xx.h>
#include <stm32h7xx_hal.h>


#define SUB_CONT(b) ((b << 1) | 0x01)   // I2C Sub-Address Continuos
#define SUB_SING(b) (b << 1)            // I2C Sub-Address Singular
#define SEND_ADDR(b) (b << 1)

#define LSM303_DEFAULT_ADDR 0b0011110          // Default Factory I2C Address

// --- LSM303 Register Map ---
#define CTRL_REG1_A 0x20
#define CTRL_REG2_A 0x21
#define CTRL_REG3_A 0x22
#define CTRL_REG4_A 0x23
#define CTRL_REG5_A 0x24
#define CTRL_REG6_A 0x25
#define REFERENCE_A 0x26
#define STATUS_REG_A 0x27
#define OUT_X_L_A 0x28
#define OUT_X_H_A 0x29
#define OUT_Y_L_A 0x2A
#define OUT_Y_H_A 0x2B
#define OUT_Z_L_A 0x2C
#define OUT_Z_H_A 0x2D
#define FIFO_CTRL_REG_A 0x2E
#define FIFO_SRC_REG_A 0x2F
#define INT1_CFG_A 0x30
#define INT1_SRC_A 0x31
#define INT1_THS_A 0x32
#define INT1_DURATION_A 0x33
#define INT2_CFG_A 0x34
#define INT2_SRC_A 0x35
#define INT2_THS_A 0x36
#define INT2_DURATION_A 0x37
#define CLICK_CFG_A 0x38
#define CLICK_SRC_A 0x39
#define CLICK_THS_A 0x3A
#define TIME_LIMIT_A 0x3B
#define TIME_LATENCY_A 0x3C
#define TIME_WINDOW_A 0x3D
#define CRA_REG_M 0x00 
#define CRB_REG_M 0x01
#define MR_REG_M 0x02
#define OUT_X_H_M 0x03
#define OUT_X_L_M 0x04
#define OUT_Z_H_M 0x05
#define OUT_Z_L_M 0x06
#define OUT_Y_H_M 0x07
#define OUT_Y_L_M 0x08
#define SR_REG_M 0x9
#define IRA_REG_M 0x0A
#define IRB_REG_M 0x0B
#define IRC_REG_M 0x0C
#define TEMP_OUT_H_M 0x31
#define TEMP_OUT_L_M 0x32

// --- LSM303 Register Bit Description
#define ODR 4
#define LPEN 3
#define ZEN 2
#define YEN 1
#define XEN 0

#define HPM 6
#define HPCF 4
#define FDS 3
#define HPCLICK 2
#define HPIS2 1
#define HPIS1 0

#define I1_CLICK 7
#define I1_AOI1 6
#define I1_AOI2 5
#define I1_DRDY1 4
#define I1_DRDY2 3
#define I1_WTM 2
#define I1_OVERRUN 1

#define BOOT 7
#define FIFO_EN 6
#define LIR_INT1 3
#define D4D_INT1 2
#define LIR_INT2 1
#define D4D_INT2 0

#define I2_CLICK_EN 7
#define I2_INT1 6
#define I2_INT2 5
#define BOOT_I1 4
#define P2_ACT 3
#define H_LACTIVE 1

#define BDU 7
#define BLE 6
#define FS 4
#define HR 3
#define SIM 0

struct lsm303 {
    uint8_t addr;
    I2C_HandleTypeDef *hi2c;
    GPIO_TypeDef *int1_port;
    uint16_t int1_port;
    GPIO_TypeDef *int2_port;
    uint16_t int2_port;
    GPIO_TypeDef *ddry_port;
    uint16_t ddry_port;
};

enum LSM303_DATARATE {
    LSM303_DATARATE_POWERDOWN = 0,
    LSM303_DATARATE_1,
    LSM303_DATARATE_10,
    LSM303_DATARATE_25,
    LSM303_DATARATE_50,
    LSM303_DATARATE_100,
    LSM303_DATARATE_200,
    LSM303_DATARATE_400,
    LSM303_DATARATE_1620,
    LSM303_DATARATE_5376
};

enum LSM303_HIGHPASS_MODE {
    LSM303_HIGHPASS_MODE_NORMAL_RESET = 0,
    LSM303_HIGHPASS_MODE_REFERENCE,
    LSM303_HIGHPASS_MODE_NORMAL,
    LSM303_HIGHPASS_MODE_AUTORESET_INTERRUPT
};

enum LSM303_FILTER_DATA_SELECTION {
    LSM303_FILTER_DATA_SELECTION_BYPASSED = 0,
    LSM303_FILTER_DATA_SELECTION_OUTPUT
};

enum LSM303_BLOCK_DATA {
    LSM303_BLOCK_DATA_CONTINOUS = 0,
    LSM303_BLOCK_DATA_QUANTUM
};

enum LSM303_ENDIANNESS {
    LSM303_BIG_ENDIAN = 0,
    LSM303_LITTLE_ENDIAN
};

enum LSM303_SCALE {
    LSM303_SCALE_2G = 0,
    LSM303_SCALE_4G,
    LSM303_SCALE_8G,
    LSM303_SCALE_16G
};

enum LSM303_HIGH_RESOLUTION {
    LSM303_HIGH_RESOLUTION_DISABLE = 0,
    LSM303_HIGH_RESOLUTION_ENABLE
};

enum LSM303_SPI_MODE {
    LSM303_SPI_MODE_4WIRE = 0,
    LSM303_SPI_MODE_3WIRE
};

enum LSM303_BOOT {
    LSM303_BOOT_NORMAL = 0,
    LSM303_REBOOT_MEMORY_CONTENT
};

enum LSM303_FIFO_EN {
    LSM303_FIFO_DISABLE = 0,
    LSM303_FIFO_ENABLE
};

enum LSM303_LATCH_INTERRUPT_REQUEST {
    LSM303_INTERRUPT_REQUEST_NOT_LATCH = 0,
    LSM303_INTERRUPT_REQUEST_LATCH
};

enum LSM303_4D_DETECTION {
    LSM303_4D_DETECTION_DISABLE = 0,
    LSM303_4D_DETECTION_ENABLE
};
enum LSM303_INTERRUPT_EN {
    LSM303_INTERRUPT_DISABLE = 0,
    LSM303_INTERRUPT_ENABLE
};

enum LSM303_REBOOT_MEM_CONTENT {
    LSM303_REBOOT_MEM_CONTENT_DISABLE = 0,
    LSM303_REBOOT_MEM_CONTENT_ENABLE
};

enum LSM303_INTERRUPT_ACTIVE {
    LSM303_INTERRUPT_ACTIVE_HIGH = 0,
    LSM303_INTERRUPT_ACTIVE_LOW
};
void _lsm303_set_addr(struct lsm303*, uint8_t addr);
HAL_StatusTypeDef _lsm303_write_singular(struct lsm303 *hlsm, uint8_t reg, uint8_t data);
HAL_StatusTypeDef _lsm303_read_singular(struct lsm303 *hlsm, uint8_t reg, uint8_t *pdata);
HAL_StatusTypeDef _lsm303_write_mult(struct lsm303 *hlsm, uint8_t reg, size_t len, uint8_t *pdata);
HAL_StatusTypeDef _lsm303_read_mult(struct lsm303 *hlsm, uint8_t reg, size_t len, uint8_t *pdata);

HAL_StatusTypeDef _lsm303_mode_normal(struct lsm303 *hlsm);
HAL_StatusTypeDef _lsm303_mode_lowpower(struct lsm303 *hlsm);
HAL_StatusTypeDef _lsm303_power_down(struct lsm303 *hlsm);
HAL_StatusTypeDef _lsm303_datarate_set(struct lsm303 *hlsm, enum LSM303_DATARATE rate);
HAL_StatusTypeDef _lsm303_xyz_set(struct lsm303 *hlsm, uint8_t setting);
HAL_StatusTypeDef _lsm303_highpass_mode_set(struct lsm303 *hlsm, enum LSM303_HIGHPASS_MODE mode);
HAL_StatusTypeDef _lsm303_highpass_filter_data_selection(struct lsm303 *hlsm, enum LSM303_FILTER_DATA_SELECTION);
HAL_StatusTypeDef _lsm303_highpass_filter_enable(struct lsm303 *hlsm, uint8_t enable);
HAL_StatusTypeDef _lsm303_interrupt(struct lsm303 *hlsm, uint8_t interrupt_en);
HAL_StatusTypeDef _lsm303_block_data_set(struct lsm303 *hlsm, enum LSM303_BLOCK_DATA);
HAL_StatusTypeDef _lsm303_endianess(struct lsm303 *hlsm, enum LSM303_ENDIANNESS);
HAL_StatusTypeDef _lsm303_scale(struct lsm303 *hlsm, enum LSM303_SCALE);
HAL_StatusTypeDef _lsm303_high_resolution(struct lsm303 *hlsm, enum LSM303_HIGH_RESOLUTION);
HAL_StatusTypeDef _lsm303_spi_mode(struct lsm303 *hlsm, enum LSM303_SPI_MODE);
HAL_StatusTypeDef _lsm303_boot_content(struct lsm303 *hlsm, enum LSM303_BOOT);
HAL_StatusTypeDef _lsm303_fifo(struct lsm303 *hlsm, enum LSM303_FIFO_EN);
HAL_StatusTypeDef _lsm303_latch_interrupt1(struct lsm303 *hlsm, enum LSM303_LATCH_INTERRUPT_REQUEST);
HAL_StatusTypeDef _lsm303_latch_interrupt2(struct lsm303 *hlsm, enum LSM303_LATCH_INTERRUPT_REQUEST);
HAL_StatusTypeDef _lsm303_4d_detection_interrupt1(struct lsm303 *hlsm, enum LSM303_4D_DETECTION);
HAL_StatusTypeDef _lsm303_4d_detection_interrupt2(struct lsm303 *hlsm, enum LSM303_4D_DETECTION);
HAL_StatusTypeDef _lsm303_click_interrupt_pad2(struct lsm303 *hlsm, enum LSM303_INTERRUPT_EN en);
HAL_StatusTypeDef _lsm303_interrupt1_pad2(struct lsm303 *hlsm, enum LSM303_INTERRUPT_EN en);
HAL_StatusTypeDef _lsm303_interrupt2_pad2(struct lsm303 *hlsm, enum LSM303_INTERRUPT_EN en);
HAL_StatusTypeDef _lsm303_boot_pad2(struct lsm303 *hlsm, bool en);
HAL_StatusTypeDef _lsm303_p2_active_function_status(struct lsm303 *hlsm, bool en);
HAL_StatusTypeDef _lsm303_interrupt_active(struct lsm303 *hlsm, enum LSM303_INTERRUPT_ACTIVE active);
HAL_StatusTypeDef _lsm303_reference_write(struct lsm303 *hlsm, uint8_t ref);
HAL_StatusTypeDef _lsm303_reference_read(struct lsm303 *hlsm, uint8_t *pref);
HAL_StatusTypeDef _lsm303_status_read(struct lsm303 *hlsm, uint8_t *pstatus);
HAL_StatusTypeDef _lsm303_accel_data(struct lsm303 *hlsm, uint8_t *pdata);
#endif
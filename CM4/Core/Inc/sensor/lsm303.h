#ifndef __LSM303_H__
#define __LSM303_H__
#include <stdbool.h>
#include <stm32h7xx.h>
#include <stm32h7xx_hal.h>
#include <stdint.h>

#define SUB_CONT(b) ((b << 1) | 0x01)   // I2C Sub-Address Continuos
#define SUB_SING(b) (b << 1)            // I2C Sub-Address Singular
#define SEND_ADDR(b) (b << 1)

// --- LSM303 Register Map ---
/* Exported constant IO ------------------------------------------------------*/
#define ACC_I2C_ADDRESS                      0x32
#define MAG_I2C_ADDRESS                      0x3C

/* Acceleration Registers */
#define LSM303DLHC_WHO_AM_I_ADDR             0x0F  /* device identification register */
#define LSM303DLHC_CTRL_REG1_A               0x20  /* Control register 1 acceleration */
#define LSM303DLHC_CTRL_REG2_A               0x21  /* Control register 2 acceleration */
#define LSM303DLHC_CTRL_REG3_A               0x22  /* Control register 3 acceleration */
#define LSM303DLHC_CTRL_REG4_A               0x23  /* Control register 4 acceleration */
#define LSM303DLHC_CTRL_REG5_A               0x24  /* Control register 5 acceleration */
#define LSM303DLHC_CTRL_REG6_A               0x25  /* Control register 6 acceleration */
#define LSM303DLHC_REFERENCE_A               0x26  /* Reference register acceleration */
#define LSM303DLHC_STATUS_REG_A              0x27  /* Status register acceleration */
#define LSM303DLHC_OUT_X_L_A                 0x28  /* Output Register X acceleration */
#define LSM303DLHC_OUT_X_H_A                 0x29  /* Output Register X acceleration */
#define LSM303DLHC_OUT_Y_L_A                 0x2A  /* Output Register Y acceleration */
#define LSM303DLHC_OUT_Y_H_A                 0x2B  /* Output Register Y acceleration */
#define LSM303DLHC_OUT_Z_L_A                 0x2C  /* Output Register Z acceleration */
#define LSM303DLHC_OUT_Z_H_A                 0x2D  /* Output Register Z acceleration */ 
#define LSM303DLHC_FIFO_CTRL_REG_A           0x2E  /* Fifo control Register acceleration */
#define LSM303DLHC_FIFO_SRC_REG_A            0x2F  /* Fifo src Register acceleration */

#define LSM303DLHC_INT1_CFG_A                0x30  /* Interrupt 1 configuration Register acceleration */
#define LSM303DLHC_INT1_SOURCE_A             0x31  /* Interrupt 1 source Register acceleration */
#define LSM303DLHC_INT1_THS_A                0x32  /* Interrupt 1 Threshold register acceleration */
#define LSM303DLHC_INT1_DURATION_A           0x33  /* Interrupt 1 DURATION register acceleration */

#define LSM303DLHC_INT2_CFG_A                0x34  /* Interrupt 2 configuration Register acceleration */
#define LSM303DLHC_INT2_SOURCE_A             0x35  /* Interrupt 2 source Register acceleration */
#define LSM303DLHC_INT2_THS_A                0x36  /* Interrupt 2 Threshold register acceleration */
#define LSM303DLHC_INT2_DURATION_A           0x37  /* Interrupt 2 DURATION register acceleration */

#define LSM303DLHC_CLICK_CFG_A               0x38  /* Click configuration Register acceleration */
#define LSM303DLHC_CLICK_SOURCE_A            0x39  /* Click 2 source Register acceleration */
#define LSM303DLHC_CLICK_THS_A               0x3A  /* Click 2 Threshold register acceleration */

#define LSM303DLHC_TIME_LIMIT_A              0x3B  /* Time Limit Register acceleration */
#define LSM303DLHC_TIME_LATENCY_A            0x3C  /* Time Latency Register acceleration */
#define LSM303DLHC_TIME_WINDOW_A             0x3D  /* Time window register acceleration */

/* Magnetic field Registers */
#define LSM303DLHC_CRA_REG_M                 0x00  /* Control register A magnetic field */
#define LSM303DLHC_CRB_REG_M                 0x01  /* Control register B magnetic field */
#define LSM303DLHC_MR_REG_M                  0x02  /* Control register MR magnetic field */
#define LSM303DLHC_OUT_X_H_M                 0x03  /* Output Register X magnetic field */
#define LSM303DLHC_OUT_X_L_M                 0x04  /* Output Register X magnetic field */
#define LSM303DLHC_OUT_Z_H_M                 0x05  /* Output Register Z magnetic field */
#define LSM303DLHC_OUT_Z_L_M                 0x06  /* Output Register Z magnetic field */ 
#define LSM303DLHC_OUT_Y_H_M                 0x07  /* Output Register Y magnetic field */
#define LSM303DLHC_OUT_Y_L_M                 0x08  /* Output Register Y magnetic field */

#define LSM303DLHC_SR_REG_M                  0x09  /* Status Register magnetic field */
#define LSM303DLHC_IRA_REG_M                 0x0A  /* IRA Register magnetic field */
#define LSM303DLHC_IRB_REG_M                 0x0B  /* IRB Register magnetic field */
#define LSM303DLHC_IRC_REG_M                 0x0C  /* IRC Register magnetic field */

#define LSM303DLHC_TEMP_OUT_H_M              0x31  /* Temperature Register magnetic field */
#define LSM303DLHC_TEMP_OUT_L_M              0x32  /* Temperature Register magnetic field */



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

#define LSM303_ID 0x33

enum LSM303_POWER {
    LSM303_NORMAL_MODE = 0x00,
    LSM303_LOWPOWER_MODE = 0x08
};

enum LSM303_DATARATE {
    LSM303_DATARATE_POWERDOWN = 0,
    LSM303_DATARATE_1 = 0x20,
    LSM303_DATARATE_10 = 0x20,
    LSM303_DATARATE_25 = 0x30,
    LSM303_DATARATE_50 = 0x40,
    LSM303_DATARATE_100 = 0x50,
    LSM303_DATARATE_200 = 0x60,
    LSM303_DATARATE_400 = 0x70,
    LSM303_DATARATE_1620 = 0x80,
    LSM303_DATARATE_5376 = 0x90
};

enum LSM303_AXES {
    LSM303_AXES_X_ENABLE = 0x01,
    LSM303_AXES_Y_ENABLE = 0x02,
    LSM303_AXES_Z_ENABLE = 0x04,
    LSM303_AXES_ENABLE  = 0x07,
    LSM303_AXES_DISABLE = 0x00
};

enum LSM303_HIGHPASS_MODE {
    LSM303_HIGHPASS_MODE_NORMAL_RESET = 0,
    LSM303_HIGHPASS_MODE_REFERENCE = 0x40,
    LSM303_HIGHPASS_MODE_NORMAL = 0x80,
    LSM303_HIGHPASS_MODE_AUTORESET_INTERRUPT = 0xC0
};

enum LSM303_HPCF {
    LSM303_HPCF_8 = 0x00,
    LSM303_HPCF_16 = 0x10,
    LSM303_HPCF_32 = 0x20,
    LSM303_HPCF_64 = 0x30
};

enum LSM303_HPFILTER {
    LSM303_HPFILTER_ENABLE = 0x08,
    LSM303_HPFILTER_DISABLE = 0x00
};

enum LSM303_HPF_CLICK {
    LSM303_HPF_CLICK_DISABLE = 0x00,
    LSM303_HPF_CLICK_ENABLE = 0x04
};

enum LSM303_HPF_AOI1 {
    LSM303_HPF_AOI1_DISABLE = 0x00,
    LSM303_HPF_AOI1_ENABLE = 0x01
};

enum LSM303_HPF_AOI2 {
    LSM303_HPF_AOI2_DISABLE = 0x00,
    LSM303_HPF_AOI2_ENABLE = 0x02
};

enum LSM303_FILTER_DATA_SELECTION {
    LSM303_FILTER_DATA_SELECTION_BYPASSED = 0,
    LSM303_FILTER_DATA_SELECTION_OUTPUT
};

enum LSM303_BLOCK_DATA {
    LSM303_BLOCK_DATA_CONTINOUS = 0,
    LSM303_BLOCK_DATA_QUANTUM = 0x80
};

enum LSM303_ENDIANNESS {
    LSM303_BIG_ENDIAN = 0x40,
    LSM303_LITTLE_ENDIAN = 0x00
};

enum LSM303_SCALE {
    LSM303_SCALE_2G = 0x00,
    LSM303_SCALE_4G = 0x10,
    LSM303_SCALE_8G = 0x20,
    LSM303_SCALE_16G = 0x30
};

enum LSM303_SENSITIVITY {
    LSM303_SENSITIVITY_2G = 1,
    LSM303_SENSITIVITY_4G = 2,
    LSM303_SENSITIVITY_8G = 4,
    LSM303_SENSITIVITY_16G = 12
};

enum LSM303_HIGH_RESOLUTION {
    LSM303_HIGH_RESOLUTION_DISABLE = 0,
    LSM303_HIGH_RESOLUTION_ENABLE = 0x08
};

enum LSM303_SPI_MODE {
    LSM303_SPI_MODE_4WIRE = 0,
    LSM303_SPI_MODE_3WIRE
};

enum LSM303_BOOT {
    LSM303_BOOT_NORMAL = 0,
    LSM303_REBOOT_MEMORY_CONTENT = 0x80
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

enum LSM303_INT1_STATUS {
    LSM303_INT1_STATUS_DISABLE = 0x00,
    LSM303_INT1_STATUS_ENABLE = 0x80
};

enum LSM303_INT1_ACTIVE {
    LSM303_INT1_ACTIVE_HIGH = 0x00,
    LSM303_INT1_ACTIVE_LOW = 0x20
};

enum LSM303_INT_AXES {
    LSM303_INT_AXES_Z_HIGH = 0x20,
    LSM303_INT_AXES_Z_LOW = 0x10,
    LSM303_INT_AXES_Y_HIGH = 0x08,
    LSM303_INT_AXES_Y_LOW = 0x04,
    LSM303_INT_AXES_X_HIGH = 0x02,
    LSM303_INT_AXES_X_LOW = 0x01  
};

enum LSM303_AXES_INT_CLICK {
    LSM303_Z_DOUBLE_CLICK = 0x20,
    LSM303_Z_SINGLE_CLICK = 0x10,
    LSM303_Y_DOUBLE_CLICK = 0x08,
    LSM303_Y_SINGLE_CLICK = 0x04,
    LSM303_X_DOUBLE_CLICK = 0x02,
    LSM303_X_SINGLE_CLICK = 0x01
};

enum LSM303_IT1 { 
    LSM303_IT1_CLICK = 0x80,
    LSM303_IT1_AOI1 = 0x40,
    LSM303_IT1_AOI2 = 0x20,
    LSM303_IT1_DRY1 = 0x10,
    LSM303_IT1_DRY2 = 0x08,
    LSM303_IT1_WTM = 0x40,
    LSM303_IT1_OVERRUN = 0x02
};

enum LSM303_IT2 {
    LSM303_IT2_CLICK = 0x80,
    LSM303_IT2_INT1 = 0x40,
    LSM303_IT2_INT2 = 0x20,
    LSM303_IT2_BOOT = 0x10,
    LSM303_IT2_ACT = 0x08,
    LSM303_IT2_HLACTIVE = 0x02
};

enum LSM303_ITCOMB {
    LSM303_ITCOMB_OR = 0x00,
    LSM303_ITCOMB_AND = 0x80,
    LSM303_MOV_RECOG = 0x40,
    LSM303_POS_RECOG = 0xC0
};

// === MAGNETIC CONFIG ====
// Measured in Hertz
enum LSM303_ODR {
    LSM303_ODR_0_75 = 0x00,
    LSM303_ODR_1_5 = 0x04,
    LSM303_ODR_3_0 = 0x08,
    LSM303_ODR_7_5 = 0x0C,
    LSM303_ODR_15 = 0x10,
    LSM303_ODR_30 = 0x14,
    LSM303_ODR_75 = 0x18,
    LSM303_ODR_220 = 0x1C
};

// Measured in Gauss
enum LSM303_FS {
    LSM303_FS_1_3 = 0x20,
    LSM303_FS_1_9 = 0x40,
    LSM303_FS_2_5 = 0x60,
    LSM303_FS_4_0 = 0x80,
    LSM303_FS_4_7 = 0xA0,
    LSM303_FS_5_6 = 0xC0,
    LSM303_FS_8_1 = 0xE0
};

enum LSM303_MSENSITIVITY {
    LSM303_MSENSITIVITY_XY_1_3 = 1100,
    LSM303_MSENSITIVITY_XY_1_9 = 855,
    LSM303_MSENSITIVITY_XY_2_5 = 670,
    LSM303_MSENSITIVITY_XY_4 = 450,
    LSM303_MSENSITIVITY_XY_4_7 = 400,
    LSM303_MSENSITIVITY_XY_5_6 = 330,
    LSM303_MSENSITIVITY_XY_8_1 = 230,
    LSM303_MSENSITIVITY_Z_1_3 = 980,
    LSM303_MSENSITIVITY_Z_1_9 = 760,
    LSM303_MSENSITIVITY_Z_2_5 = 600,
    LSM303_MSENSITIVITY_Z_4 = 400,
    LSM303_MSENSITIVITY_Z_4_7 = 355,
    LSM303_MSENSITIVITY_Z_5_6 = 295,
    LSM303_MSENSITIVITY_Z_8_1 = 205
};

enum LSM303_MMODE {
    LSM303_MMODE_CONTINUOS = 0x00,
    LSM303_MMODE_SINGLE = 0x01,
    LSM303_MSLEEP = 0x02
};

enum LSM303_TEMP {
    LSM303_TEMP_ENABLE = 0x80,
    LSM303_TEMP_DISABLE = 0x00
};


uint32_t __lsm303_axwrite_single(uint8_t reg, uint8_t data);
uint32_t __lsm303_axread_single(uint8_t reg, uint8_t *pdata);
uint32_t __lsm303_axwrite( uint8_t reg, size_t len, uint8_t *pdata);
uint32_t __lsm303_axread(uint8_t reg, size_t len, uint8_t *pdata);

uint32_t __lsm303_mwrite_single(uint8_t reg, uint8_t data);
uint32_t __lsm303_mread_single(uint8_t reg, uint8_t *pdata);
uint32_t __lsm303_mwrite( uint8_t reg, size_t len, uint8_t *pdata);
uint32_t __lsm303_mread(uint8_t reg, size_t len, uint8_t *pdata);

uint32_t _lsm303_mode_normal();
uint32_t _lsm303_mode_lowpower();
uint32_t _lsm303_power_down();
uint32_t _lsm303_datarate_set(enum LSM303_DATARATE rate);
uint32_t _lsm303_xyz_set(uint8_t setting);
uint32_t _lsm303_highpass_mode_set(enum LSM303_HIGHPASS_MODE mode);
uint32_t _lsm303_highpass_filter_data_selection(enum LSM303_FILTER_DATA_SELECTION);
uint32_t _lsm303_highpass_filter_enable(uint8_t enable);
uint32_t _lsm303_interrupt(uint8_t interrupt_en);
uint32_t _lsm303_block_data_set(enum LSM303_BLOCK_DATA);
uint32_t _lsm303_endianess(enum LSM303_ENDIANNESS);
uint32_t _lsm303_scale(enum LSM303_SCALE);
uint32_t _lsm303_high_resolution(enum LSM303_HIGH_RESOLUTION);
uint32_t _lsm303_spi_mode(enum LSM303_SPI_MODE);
uint32_t _lsm303_boot_content(enum LSM303_BOOT);
uint32_t _lsm303_fifo(enum LSM303_FIFO_EN);
uint32_t _lsm303_latch_interrupt1(enum LSM303_LATCH_INTERRUPT_REQUEST);
uint32_t _lsm303_latch_interrupt2(enum LSM303_LATCH_INTERRUPT_REQUEST);
uint32_t _lsm303_4d_detection_interrupt1(enum LSM303_4D_DETECTION);
uint32_t _lsm303_4d_detection_interrupt2(enum LSM303_4D_DETECTION);
uint32_t _lsm303_click_interrupt_pad2(enum LSM303_INTERRUPT_EN en);
uint32_t _lsm303_interrupt1_pad2(enum LSM303_INTERRUPT_EN en);
uint32_t _lsm303_interrupt2_pad2(enum LSM303_INTERRUPT_EN en);
uint32_t _lsm303_boot_pad2(bool en);
uint32_t _lsm303_p2_active_function_status(bool en);
uint32_t _lsm303_interrupt_active(enum LSM303_INTERRUPT_ACTIVE active);
uint32_t _lsm303_reference_write(uint8_t ref);
uint32_t _lsm303_reference_read(uint8_t *pref);
uint32_t _lsm303_status_read(uint8_t *pstatus);
uint32_t _lsm303_accel_data(uint8_t *pdata);
#endif
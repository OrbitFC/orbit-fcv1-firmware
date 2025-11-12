#include <sensor/lsm303.h>
#include "task_queue.h"
#include "task_semphr.h"
#include "debug.h"

#define LSM303_WAIT_MS 10       // Each queue entry or i2c request will wait for at most LSM303_WAIT_MS milliseconds
#define LSM303_WAIT_RETRY 2     // Retry at most LSM303_WAIT_RETRY time(s) for stuck i2c queue
// Each instance of LSM303 usage have to define a struct i2cRequest
// Since only sensorTask use this we wouldn't have to worry
static struct i2cRequest req;

#define LSM303_INSTANCE I2C_INSTANCE_I2C2
uint32_t __lsm303_req()
{
    BaseType_t t = xQueueSendToBack(i2cQueue, &req, pdMS_TO_TICKS(LSM303_WAIT_MS));
    if (t == errQUEUE_FULL) {
        for (unsigned i = 0; i < LSM303_WAIT_RETRY; i++) {
            vTaskDelay(pdMS_TO_TICKS(LSM303_WAIT_MS));
            t = xQueueSendToBack(i2cQueue, &req, pdMS_TO_TICKS(LSM303_WAIT_MS));
            if (t == pdTRUE)
                break;
        }
        if (t != pdTRUE) {
            return STATUS_QUEUE_FULL;
        }
    }
    t = xSemaphoreTake(sensorI2cSemphr, pdMS_TO_TICKS(LSM303_WAIT_MS));
    if (t != pdTRUE)
        return STATUS_NO_QUEUE_RESPONSE;
    return STATUS_OK;
}

uint32_t __lsm303_axwrite_single(uint8_t reg, uint8_t data)
{
    req.dev_addr = ACC_I2C_ADDRESS;
    req.mem_addr = reg;
    req.tx = &data;
    req.inst = LSM303_INSTANCE;
    req.size = 1;
    req.type = I2C_REQUEST_WRITE;
    req.semphr = sensorI2cSemphr;
    
    return __lsm303_req();

}

uint32_t _lsm303_axread_single(uint8_t reg, uint8_t *pdata)
{
    req.dev_addr = ACC_I2C_ADDRESS;
    req.mem_addr = reg;
    req.rx = pdata;
    req.inst = LSM303_INSTANCE;
    req.size = 1;
    req.type = I2C_REQUEST_READ;
    req.semphr = sensorI2cSemphr;
    
    __lsm303_req();
}

uint32_t _lsm303_axwrite(uint8_t reg, size_t size, uint8_t *pdata)
{
    req.dev_addr = ACC_I2C_ADDRESS;
    req.mem_addr = reg;
    req.tx = pdata;
    req.inst = LSM303_INSTANCE;
    req.size = size;
    req.type = I2C_REQUEST_WRITE;
    req.semphr = sensorI2cSemphr;
    
    __lsm303_req();
}

uint32_t _lsm303_axread(uint8_t reg, size_t size, uint8_t *pdata)
{
    req.dev_addr = ACC_I2C_ADDRESS;
    req.mem_addr = reg;
    req.tx = pdata;
    req.inst = LSM303_INSTANCE;
    req.size = size;
    req.type = I2C_REQUEST_READ;
    req.semphr = sensorI2cSemphr;
    
    __lsm303_req();
}

uint32_t __lsm303_mwrite_single(uint8_t reg, uint8_t data)
{
    req.dev_addr = MAG_I2C_ADDRESS;
    req.mem_addr = reg;
    req.tx = &data;
    req.inst = LSM303_INSTANCE;
    req.size = 1;
    req.type = I2C_REQUEST_WRITE;
    req.semphr = sensorI2cSemphr;
    
    __lsm303_req();
}

uint32_t __lsm303_mread_single(uint8_t reg, uint8_t *pdata)
{
    req.dev_addr = MAG_I2C_ADDRESS;
    req.mem_addr = reg;
    req.tx = pdata;
    req.inst = LSM303_INSTANCE;
    req.size = 1;
    req.type = I2C_REQUEST_READ;
    req.semphr = sensorI2cSemphr;

    __lsm303_req();
}

uint32_t __lsm303_mwrite( uint8_t reg, size_t size, uint8_t *pdata)
{
    req.dev_addr = MAG_I2C_ADDRESS;
    req.mem_addr = reg;
    req.tx = pdata;
    req.inst = LSM303_INSTANCE;
    req.size = size;
    req.type = I2C_REQUEST_WRITE;
    req.semphr = sensorI2cSemphr;

    __lsm303_req();
}

uint32_t __lsm303_mread(uint8_t reg, size_t size, uint8_t *pdata)
{
    req.dev_addr = MAG_I2C_ADDRESS;
    req.mem_addr = reg;
    req.tx = pdata;
    req.inst = LSM303_INSTANCE;
    req.size = size;
    req.type = I2C_REQUEST_READ;
    req.semphr = sensorI2cSemphr;

    __lsm303_req();
}

uint32_t _lsm303_mode_normal(struct lsm303 *hlsm)
{
    uint32_t status;
    static uint8_t buf[4];
    status = _lsm303_read_mult(hlsm, CTRL_REG1_A, 4, buf);
    if (status != HAL_OK)
        return status;
    buf[0] &= ~(1U << LPEN);
    buf[3] |= (1U << HR);
    return _lsm303_write_mult(hlsm, CTRL_REG1_A, 4, buf); // We are guaranteed that the Control Register does not change beside our will
}

uint32_t _lsm303_mode_lowpower(struct lsm303 *hlsm)
{
    uint32_t status;
    static uint8_t buf[4];
    status = _lsm303_read_mult(hlsm, CTRL_REG1_A, 4, buf);
    if (status != HAL_OK)
        return status;
    buf[0] |= (1U << LPEN);
    buf[3] &= ~(1U << HR);
    return _lsm303_write_mult(hlsm, CTRL_REG1_A, 4, buf);    
}

uint32_t _lsm303_power_down(struct lsm303 *hlsm)
{
    uint32_t status;
    uint8_t ctrl_reg_1;
    status = _lsm303_read_singular(hlsm, CTRL_REG1_A, &ctrl_reg_1);
    if (status != HAL_OK)
        return status;
    ctrl_reg_1 &= (0b1111 << ODR);
    return _lsm303_write_singular(hlsm, CTRL_REG1_A, ctrl_reg_1);
}

uint32_t _lsm303_datarate_set(struct lsm303 *hlsm, enum LSM303_DATARATE rate)
{
    uint32_t status;
    uint8_t ctrl_reg_1;
    status = _lsm303_read_singular(hlsm, CTRL_REG1_A, &ctrl_reg_1);
    if (status != HAL_OK)
        return status;
    ctrl_reg_1 &= 0x0F;
    ctrl_reg_1 += (rate << ODR);
    return _lsm303_write_singular(hlsm, CTRL_REG1_A, ctrl_reg_1);
}
/***
 * @brief Enable X, Y, Z measurement
 * @param settings Set bit 0 for X, bit 1 for Y and bit 2 for Z
 */
uint32_t _lsm303_xyz_set(struct lsm303 *hlsm, uint8_t setting)
{
    uint32_t status;
    uint8_t ctrl_reg_1;
    status = _lsm303_read_singular(hlsm, CTRL_REG1_A, &ctrl_reg_1);
    if (status != HAL_OK)
        return status;
    ctrl_reg_1 &= 0xF8;
    ctrl_reg_1 += (setting << XEN);
    return _lsm303_write_singular(hlsm, CTRL_REG1_A, ctrl_reg_1);
}

uint32_t _lsm303_highpass_mode_set(struct lsm303 *hlsm, enum LSM303_HIGHPASS_MODE mode)
{
    uint32_t status;
    uint8_t ctrl_reg_2;
    status = _lsm303_read_singular(hlsm, CTRL_REG2_A, &ctrl_reg_2);
    if (status != HAL_OK)
        return status;
    ctrl_reg_2 &= 0x4F;
    ctrl_reg_2 += ((uint8_t)mode << HPM);
    return _lsm303_write_singular(hlsm, CTRL_REG2_A, ctrl_reg_2);
}

uint32_t _lsm303_highpass_filter_data_selection(struct lsm303 *hlsm, 
    enum LSM303_FILTER_DATA_SELECTION selection)
{
    uint32_t status;
    uint8_t ctrl_reg_2;
    status = _lsm303_read_singular(hlsm, CTRL_REG2_A, &ctrl_reg_2);
    if (status != HAL_OK)
        return status;
    ctrl_reg_2 &= ~(1 << FDS);
    ctrl_reg_2 += ((uint8_t)selection << FDS);
    return _lsm303_write_singular(hlsm, CTRL_REG2_A, ctrl_reg_2);
}
/**
 * @param enable Set bit 2 for click function, bit 1 for interrupt 2 and bit 0 for interrupt 1
 */
uint32_t _lsm303_highpass_filter_enable(struct lsm303 *hlsm, uint8_t enable)
{
    uint32_t status;
    uint8_t ctrl_reg_2;
    status = _lsm303_read_singular(hlsm, CTRL_REG2_A, &ctrl_reg_2);
    if (status != HAL_OK)
        return status;
    ctrl_reg_2 &= 0xF8;
    ctrl_reg_2 += ((uint8_t)enable << HPIS1);
    return _lsm303_write_singular(hlsm, CTRL_REG2_A, ctrl_reg_2);
}

/**
 * 
 */
uint32_t _lsm303_interrupt(struct lsm303 *hlsm, uint8_t interrupt_en)
{
    return _lsm303_write_singular(hlsm, CTRL_REG3_A, interrupt_en);   
}

uint32_t _lsm303_block_data_set(struct lsm303 *hlsm, enum LSM303_BLOCK_DATA blk)
{
    uint32_t status;
    uint8_t ctrl_reg_4;
    status = _lsm303_read_singular(hlsm, CTRL_REG4_A, &ctrl_reg_4);
    if (status != HAL_OK)
        return status;
    ctrl_reg_4 &= ~(1 << BDU);
    ctrl_reg_4 += (blk << BDU);
    return _lsm303_write_singular(hlsm, CTRL_REG4_A, ctrl_reg_4);    
}

uint32_t _lsm303_endianess(struct lsm303 *hlsm, enum LSM303_ENDIANNESS endianness)
{
    uint32_t status;
    uint8_t ctrl_reg_4;
    status = _lsm303_read_singular(hlsm, CTRL_REG4_A, &ctrl_reg_4);
    if (status != HAL_OK)
        return status;
    ctrl_reg_4 &= ~(1 << BLE);
    ctrl_reg_4 += (endianness << BLE);
    return _lsm303_write_singular(hlsm, CTRL_REG4_A, ctrl_reg_4);    
}

uint32_t _lsm303_scale(struct lsm303 *hlsm, enum LSM303_SCALE scale)
{
    uint32_t status;
    uint8_t ctrl_reg_4;
    status = _lsm303_read_singular(hlsm, CTRL_REG4_A, &ctrl_reg_4);
    if (status != HAL_OK)
        return status;
    ctrl_reg_4 &= 0xCF;
    ctrl_reg_4 += (scale << FS);
    return _lsm303_write_singular(hlsm, CTRL_REG4_A, ctrl_reg_4);
}

uint32_t _lsm303_high_resolution(struct lsm303 *hlsm, enum LSM303_HIGH_RESOLUTION res)
{
    uint32_t status;
    uint8_t ctrl_reg_4;
    status = _lsm303_read_singular(hlsm, CTRL_REG4_A, &ctrl_reg_4);
    if (status != HAL_OK)
        return status;
    ctrl_reg_4 &= ~(1 << HR);
    ctrl_reg_4 += (res<< HR);
    return _lsm303_write_singular(hlsm, CTRL_REG4_A, ctrl_reg_4);
}

uint32_t _lsm303_spi_mode(struct lsm303 *hlsm, enum LSM303_SPI_MODE mode)
{
    uint32_t status;
    uint8_t ctrl_reg_4;
    status = _lsm303_read_singular(hlsm, CTRL_REG4_A, &ctrl_reg_4);
    if (status != HAL_OK)
        return status;
    ctrl_reg_4 &= ~(1 << SIM);
    ctrl_reg_4 += (mode << SIM);
    return _lsm303_write_singular(hlsm, CTRL_REG4_A, ctrl_reg_4);    
}

uint32_t _lsm303_boot_content(struct lsm303 *hlsm, enum LSM303_BOOT boot)
{
    uint32_t status;
    uint8_t ctrl_reg_5;
    status = _lsm303_read_singular(hlsm, CTRL_REG5_A, &ctrl_reg_5);
    if (status != HAL_OK)
        return status;
    ctrl_reg_5 &= ~(1 << BOOT);
    ctrl_reg_5 += (boot << BOOT);
    return _lsm303_write_singular(hlsm, CTRL_REG5_A, ctrl_reg_5); 
}

uint32_t _lsm303_fifo(struct lsm303 *hlsm, enum LSM303_FIFO_EN fifo)
{
    uint32_t status;
    uint8_t ctrl_reg_5;
    status = _lsm303_read_singular(hlsm, CTRL_REG5_A, &ctrl_reg_5);
    if (status != HAL_OK)
        return status;
    ctrl_reg_5 &= ~(1 << FIFO_EN);
    ctrl_reg_5 += (fifo << FIFO_EN);
    return _lsm303_write_singular(hlsm, CTRL_REG5_A, ctrl_reg_5);     
}

uint32_t _lsm303_latch_interrupt1(struct lsm303 *hlsm, enum LSM303_LATCH_INTERRUPT_REQUEST latch)
{
    uint32_t status;
    uint8_t ctrl_reg_5;
    status = _lsm303_read_singular(hlsm, CTRL_REG5_A, &ctrl_reg_5);
    if (status != HAL_OK)
        return status;
    ctrl_reg_5 &= ~(1 << LIR_INT1);
    ctrl_reg_5 += (latch << LIR_INT1);
    return _lsm303_write_singular(hlsm, CTRL_REG5_A, ctrl_reg_5);   
}

uint32_t _lsm303_latch_interrupt2(struct lsm303 *hlsm, enum LSM303_LATCH_INTERRUPT_REQUEST latch)
{
    uint32_t status;
    uint8_t ctrl_reg_5;
    status = _lsm303_read_singular(hlsm, CTRL_REG5_A, &ctrl_reg_5);
    if (status != HAL_OK)
        return status;
    ctrl_reg_5 &= ~(1 << LIR_INT2);
    ctrl_reg_5 += (latch << LIR_INT2);
    return _lsm303_write_singular(hlsm, CTRL_REG5_A, ctrl_reg_5);   
}

uint32_t _lsm303_4d_detection_interrupt1(struct lsm303 *hlsm, enum LSM303_4D_DETECTION d4d)
{
    uint32_t status;
    uint8_t ctrl_reg_5;
    status = _lsm303_read_singular(hlsm, CTRL_REG5_A, &ctrl_reg_5);
    if (status != HAL_OK)
        return status;
    ctrl_reg_5 &= ~(1 << D4D_INT1);
    ctrl_reg_5 += (d4d << D4D_INT1);
    return _lsm303_write_singular(hlsm, CTRL_REG5_A, ctrl_reg_5);      
}

uint32_t _lsm303_4d_detection_interrupt2(struct lsm303 *hlsm, enum LSM303_4D_DETECTION d4d)
{
    uint32_t status;
    uint8_t ctrl_reg_5;
    status = _lsm303_read_singular(hlsm, CTRL_REG5_A, &ctrl_reg_5);
    if (status != HAL_OK)
        return status;
    ctrl_reg_5 &= ~(1 << D4D_INT1);
    ctrl_reg_5 += (d4d << D4D_INT1);
    return _lsm303_write_singular(hlsm, CTRL_REG5_A, ctrl_reg_5);      
}

uint32_t _lsm303_click_interrupt_pad2(struct lsm303 *hlsm, enum LSM303_INTERRUPT_EN en)
{
    uint32_t status;
    uint8_t ctrl_reg_6;
    status = _lsm303_read_singular(hlsm, CTRL_REG6_A, &ctrl_reg_6);
    if (status != HAL_OK)
        return status;
    ctrl_reg_6 &= ~(1 << I2_CLICK_EN);
    ctrl_reg_6 += (en << I2_CLICK_EN);
    return _lsm303_write_singular(hlsm, CTRL_REG6_A, ctrl_reg_6);          
}

uint32_t _lsm303_interrupt1_pad2(struct lsm303 *hlsm, enum LSM303_INTERRUPT_EN en)
{
    uint32_t status;
    uint8_t ctrl_reg_6;
    status = _lsm303_read_singular(hlsm, CTRL_REG6_A, &ctrl_reg_6);
    if (status != HAL_OK)
        return status;
    ctrl_reg_6 &= ~(1 << I2_INT1);
    ctrl_reg_6 += (en << I2_INT1);
    return _lsm303_write_singular(hlsm, CTRL_REG6_A, ctrl_reg_6); 
}

uint32_t _lsm303_interrupt2_pad2(struct lsm303 *hlsm, enum LSM303_INTERRUPT_EN en)
{
    uint32_t status;
    uint8_t ctrl_reg_6;
    status = _lsm303_read_singular(hlsm, CTRL_REG6_A, &ctrl_reg_6);
    if (status != HAL_OK)
        return status;
    ctrl_reg_6 &= ~(1 << I2_INT2);
    ctrl_reg_6 += (en << I2_INT2);
    return _lsm303_write_singular(hlsm, CTRL_REG6_A, ctrl_reg_6); 
}

uint32_t _lsm303_boot_pad2(struct lsm303 *hlsm, bool en)
{
    uint32_t status;
    uint8_t ctrl_reg_6;
    status = _lsm303_read_singular(hlsm, CTRL_REG6_A, &ctrl_reg_6);
    if (status != HAL_OK)
        return status;
    ctrl_reg_6 &= ~(1 << BOOT_I1);
    ctrl_reg_6 += (en << BOOT_I1);
    return _lsm303_write_singular(hlsm, CTRL_REG6_A, ctrl_reg_6); 
}

uint32_t _lsm303_p2_active_function_status(struct lsm303 *hlsm, bool en)
{
    uint32_t status;
    uint8_t ctrl_reg_6;
    status = _lsm303_read_singular(hlsm, CTRL_REG6_A, &ctrl_reg_6);
    if (status != HAL_OK)
        return status;
    ctrl_reg_6 &= ~(1 << P2_ACT);
    ctrl_reg_6 += (en << P2_ACT);
    return _lsm303_write_singular(hlsm, CTRL_REG6_A, ctrl_reg_6); 
}

uint32_t _lsm303_interrupt_active(struct lsm303 *hlsm, enum LSM303_INTERRUPT_ACTIVE active)
{
    uint32_t status;
    uint8_t ctrl_reg_6;
    status = _lsm303_read_singular(hlsm, CTRL_REG6_A, &ctrl_reg_6);
    if (status != HAL_OK)
        return status;
    ctrl_reg_6 &= ~(1 << H_LACTIVE);
    ctrl_reg_6 += (active << H_LACTIVE);
    return _lsm303_write_singular(hlsm, CTRL_REG6_A, ctrl_reg_6); 
}

uint32_t _lsm303_reference_write(struct lsm303 *hlsm, uint8_t ref)
{
    return _lsm303_write_singular(hlsm, REFERENCE_A, ref);
}

uint32_t _lsm303_reference_read(struct lsm303 *hlsm, uint8_t *pref)
{
    return _lsm303_read_singular(hlsm, REFERENCE_A, pref);
}

uint32_t _lsm303_status_read(struct lsm303 *hlsm, uint8_t *pstatus)
{
    return _lsm303_read_singular(hlsm, STATUS_REG_A, pstatus);
}

uint32_t _lsm303_accel_data(struct lsm303 *hlsm, uint8_t *pdata)
{
    return _lsm303_read_mult(hlsm, OUT_X_L_A, 6, pdata);
}
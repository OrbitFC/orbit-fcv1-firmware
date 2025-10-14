#include <sensor/lsm303.h>

HAL_StatusTypeDef _lsm303_write_singular(struct lsm303 *hlsm, uint8_t reg, uint8_t data)
{
    return HAL_I2C_Mem_Write_DMA(hlsm->hi2c, hlsm->addr, SUB_SING(reg), 
    I2C_MEMADD_SIZE_8BIT, &data, 1);
}

HAL_StatusTypeDef _lsm303_read_singular(struct lsm303 *hlsm, uint8_t reg, uint8_t *pdata)
{
    return HAL_I2C_Mem_Read_DMA(hlsm->hi2c, hlsm->addr, SUB_SING(reg),
    I2C_MEMADD_SIZE_8BIT, pdata, 1);
}

HAL_StatusTypeDef _lsm303_write_mult(struct lsm303 *hlsm, uint8_t reg, size_t len, uint8_t *pdata)
{
    return HAL_I2C_Mem_Write_DMA(hlsm->hi2c, hlsm->addr, SUB_CONT(reg),
    I2C_MEMADD_SIZE_8BIT, pdata, len);
}

HAL_StatusTypeDef _lsm303_read_mult(struct lsm303 *hlsm, uint8_t reg, size_t len, uint8_t *pdata)
{
    return HAL_I2C_Mem_Read_DMA(hlsm->hi2c, hlsm->addr, SUB_CONT(reg),
    I2C_MEMADD_SIZE_8BIT, pdata, len);
}

void _lsm303_set_addr(struct lsm303 *hlsm, uint8_t addr)
{
    hlsm->addr = SEND_ADDR(addr);
}

HAL_StatusTypeDef _lsm303_mode_normal(struct lsm303 *hlsm)
{
    HAL_StatusTypeDef status;
    static uint8_t buf[4];
    status = _lsm303_read_mult(hlsm, CTRL_REG1_A, 4, buf);
    if (status != HAL_OK)
        return status;
    buf[0] &= ~(1U << LPEN);
    buf[3] |= (1U << HR);
    return _lsm303_write_mult(hlsm, CTRL_REG1_A, 4, buf); // We are guaranteed that the Control Register does not change beside our will
}

HAL_StatusTypeDef _lsm303_mode_lowpower(struct lsm303 *hlsm)
{
    HAL_StatusTypeDef status;
    static uint8_t buf[4];
    status = _lsm303_read_mult(hlsm, CTRL_REG1_A, 4, buf);
    if (status != HAL_OK)
        return status;
    buf[0] |= (1U << LPEN);
    buf[3] &= ~(1U << HR);
    return _lsm303_write_mult(hlsm, CTRL_REG1_A, 4, buf);    
}

HAL_StatusTypeDef _lsm303_power_down(struct lsm303 *hlsm)
{
    HAL_StatusTypeDef status;
    uint8_t ctrl_reg_1;
    status = _lsm303_read_singular(hlsm, CTRL_REG1_A, &ctrl_reg_1);
    if (status != HAL_OK)
        return status;
    ctrl_reg_1 &= (0b1111 << ODR);
    return _lsm303_write_singular(hlsm, CTRL_REG1_A, ctrl_reg_1);
}

HAL_StatusTypeDef _lsm303_datarate_set(struct lsm303 *hlsm, enum LSM303_DATARATE rate)
{
    HAL_StatusTypeDef status;
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
HAL_StatusTypeDef _lsm303_xyz_set(struct lsm303 *hlsm, uint8_t setting)
{
    HAL_StatusTypeDef status;
    uint8_t ctrl_reg_1;
    status = _lsm303_read_singular(hlsm, CTRL_REG1_A, &ctrl_reg_1);
    if (status != HAL_OK)
        return status;
    ctrl_reg_1 &= 0xF8;
    ctrl_reg_1 += (setting << XEN);
    return _lsm303_write_singular(hlsm, CTRL_REG1_A, ctrl_reg_1);
}

HAL_StatusTypeDef _lsm303_highpass_mode_set(struct lsm303 *hlsm, enum LSM303_HIGHPASS_MODE mode)
{
    HAL_StatusTypeDef status;
    uint8_t ctrl_reg_2;
    status = _lsm303_read_singular(hlsm, CTRL_REG2_A, &ctrl_reg_2);
    if (status != HAL_OK)
        return status;
    ctrl_reg_2 &= 0x4F;
    ctrl_reg_2 += ((uint8_t)mode << HPM);
    return _lsm303_write_singular(hlsm, CTRL_REG2_A, ctrl_reg_2);
}

HAL_StatusTypeDef _lsm303_highpass_filter_data_selection(struct lsm303 *hlsm, 
    enum LSM303_FILTER_DATA_SELECTION selection)
{
    HAL_StatusTypeDef status;
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
HAL_StatusTypeDef _lsm303_highpass_filter_enable(struct lsm303 *hlsm, uint8_t enable)
{
    HAL_StatusTypeDef status;
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
HAL_StatusTypeDef _lsm303_interrupt(struct lsm303 *hlsm, uint8_t interrupt_en)
{
    return _lsm303_write_singular(hlsm, CTRL_REG3_A, interrupt_en);   
}

HAL_StatusTypeDef _lsm303_block_data_set(struct lsm303 *hlsm, enum LSM303_BLOCK_DATA blk)
{
    HAL_StatusTypeDef status;
    uint8_t ctrl_reg_4;
    status = _lsm303_read_singular(hlsm, CTRL_REG4_A, &ctrl_reg_4);
    if (status != HAL_OK)
        return status;
    ctrl_reg_4 &= ~(1 << BDU);
    ctrl_reg_4 += (blk << BDU);
    return _lsm303_write_singular(hlsm, CTRL_REG4_A, ctrl_reg_4);    
}

HAL_StatusTypeDef _lsm303_endianess(struct lsm303 *hlsm, enum LSM303_ENDIANNESS endianness)
{
    HAL_StatusTypeDef status;
    uint8_t ctrl_reg_4;
    status = _lsm303_read_singular(hlsm, CTRL_REG4_A, &ctrl_reg_4);
    if (status != HAL_OK)
        return status;
    ctrl_reg_4 &= ~(1 << BLE);
    ctrl_reg_4 += (endianness << BLE);
    return _lsm303_write_singular(hlsm, CTRL_REG4_A, ctrl_reg_4);    
}

HAL_StatusTypeDef _lsm303_scale(struct lsm303 *hlsm, enum LSM303_SCALE scale)
{
    HAL_StatusTypeDef status;
    uint8_t ctrl_reg_4;
    status = _lsm303_read_singular(hlsm, CTRL_REG4_A, &ctrl_reg_4);
    if (status != HAL_OK)
        return status;
    ctrl_reg_4 &= 0xCF;
    ctrl_reg_4 += (scale << FS);
    return _lsm303_write_singular(hlsm, CTRL_REG4_A, ctrl_reg_4);
}

HAL_StatusTypeDef _lsm303_high_resolution(struct lsm303 *hlsm, enum LSM303_HIGH_RESOLUTION res)
{
    HAL_StatusTypeDef status;
    uint8_t ctrl_reg_4;
    status = _lsm303_read_singular(hlsm, CTRL_REG4_A, &ctrl_reg_4);
    if (status != HAL_OK)
        return status;
    ctrl_reg_4 &= ~(1 << HR);
    ctrl_reg_4 += (res<< HR);
    return _lsm303_write_singular(hlsm, CTRL_REG4_A, ctrl_reg_4);
}

HAL_StatusTypeDef _lsm303_spi_mode(struct lsm303 *hlsm, enum LSM303_SPI_MODE mode)
{
    HAL_StatusTypeDef status;
    uint8_t ctrl_reg_4;
    status = _lsm303_read_singular(hlsm, CTRL_REG4_A, &ctrl_reg_4);
    if (status != HAL_OK)
        return status;
    ctrl_reg_4 &= ~(1 << SIM);
    ctrl_reg_4 += (mode << SIM);
    return _lsm303_write_singular(hlsm, CTRL_REG4_A, ctrl_reg_4);    
}

HAL_StatusTypeDef _lsm303_boot_content(struct lsm303 *hlsm, enum LSM303_BOOT boot)
{
    HAL_StatusTypeDef status;
    uint8_t ctrl_reg_5;
    status = _lsm303_read_singular(hlsm, CTRL_REG5_A, &ctrl_reg_5);
    if (status != HAL_OK)
        return status;
    ctrl_reg_5 &= ~(1 << BOOT);
    ctrl_reg_5 += (boot << BOOT);
    return _lsm303_write_singular(hlsm, CTRL_REG5_A, ctrl_reg_5); 
}

HAL_StatusTypeDef _lsm303_fifo(struct lsm303 *hlsm, enum LSM303_FIFO_EN fifo)
{
    HAL_StatusTypeDef status;
    uint8_t ctrl_reg_5;
    status = _lsm303_read_singular(hlsm, CTRL_REG5_A, &ctrl_reg_5);
    if (status != HAL_OK)
        return status;
    ctrl_reg_5 &= ~(1 << FIFO_EN);
    ctrl_reg_5 += (fifo << FIFO_EN);
    return _lsm303_write_singular(hlsm, CTRL_REG5_A, ctrl_reg_5);     
}

HAL_StatusTypeDef _lsm303_latch_interrupt1(struct lsm303 *hlsm, enum LSM303_LATCH_INTERRUPT_REQUEST latch)
{
    HAL_StatusTypeDef status;
    uint8_t ctrl_reg_5;
    status = _lsm303_read_singular(hlsm, CTRL_REG5_A, &ctrl_reg_5);
    if (status != HAL_OK)
        return status;
    ctrl_reg_5 &= ~(1 << LIR_INT1);
    ctrl_reg_5 += (latch << LIR_INT1);
    return _lsm303_write_singular(hlsm, CTRL_REG5_A, ctrl_reg_5);   
}

HAL_StatusTypeDef _lsm303_latch_interrupt2(struct lsm303 *hlsm, enum LSM303_LATCH_INTERRUPT_REQUEST latch)
{
    HAL_StatusTypeDef status;
    uint8_t ctrl_reg_5;
    status = _lsm303_read_singular(hlsm, CTRL_REG5_A, &ctrl_reg_5);
    if (status != HAL_OK)
        return status;
    ctrl_reg_5 &= ~(1 << LIR_INT2);
    ctrl_reg_5 += (latch << LIR_INT2);
    return _lsm303_write_singular(hlsm, CTRL_REG5_A, ctrl_reg_5);   
}

HAL_StatusTypeDef _lsm303_4d_detection_interrupt1(struct lsm303 *hlsm, enum LSM303_4D_DETECTION d4d)
{
    HAL_StatusTypeDef status;
    uint8_t ctrl_reg_5;
    status = _lsm303_read_singular(hlsm, CTRL_REG5_A, &ctrl_reg_5);
    if (status != HAL_OK)
        return status;
    ctrl_reg_5 &= ~(1 << D4D_INT1);
    ctrl_reg_5 += (d4d << D4D_INT1);
    return _lsm303_write_singular(hlsm, CTRL_REG5_A, ctrl_reg_5);      
}

HAL_StatusTypeDef _lsm303_4d_detection_interrupt2(struct lsm303 *hlsm, enum LSM303_4D_DETECTION d4d)
{
    HAL_StatusTypeDef status;
    uint8_t ctrl_reg_5;
    status = _lsm303_read_singular(hlsm, CTRL_REG5_A, &ctrl_reg_5);
    if (status != HAL_OK)
        return status;
    ctrl_reg_5 &= ~(1 << D4D_INT1);
    ctrl_reg_5 += (d4d << D4D_INT1);
    return _lsm303_write_singular(hlsm, CTRL_REG5_A, ctrl_reg_5);      
}

HAL_StatusTypeDef _lsm303_click_interrupt_pad2(struct lsm303 *hlsm, enum LSM303_INTERRUPT_EN en)
{
    HAL_StatusTypeDef status;
    uint8_t ctrl_reg_6;
    status = _lsm303_read_singular(hlsm, CTRL_REG6_A, &ctrl_reg_6);
    if (status != HAL_OK)
        return status;
    ctrl_reg_6 &= ~(1 << I2_CLICK_EN);
    ctrl_reg_6 += (en << I2_CLICK_EN);
    return _lsm303_write_singular(hlsm, CTRL_REG6_A, ctrl_reg_6);          
}

HAL_StatusTypeDef _lsm303_interrupt1_pad2(struct lsm303 *hlsm, enum LSM303_INTERRUPT_EN en)
{
    HAL_StatusTypeDef status;
    uint8_t ctrl_reg_6;
    status = _lsm303_read_singular(hlsm, CTRL_REG6_A, &ctrl_reg_6);
    if (status != HAL_OK)
        return status;
    ctrl_reg_6 &= ~(1 << I2_INT1);
    ctrl_reg_6 += (en << I2_INT1);
    return _lsm303_write_singular(hlsm, CTRL_REG6_A, ctrl_reg_6); 
}

HAL_StatusTypeDef _lsm303_interrupt2_pad2(struct lsm303 *hlsm, enum LSM303_INTERRUPT_EN en)
{
    HAL_StatusTypeDef status;
    uint8_t ctrl_reg_6;
    status = _lsm303_read_singular(hlsm, CTRL_REG6_A, &ctrl_reg_6);
    if (status != HAL_OK)
        return status;
    ctrl_reg_6 &= ~(1 << I2_INT2);
    ctrl_reg_6 += (en << I2_INT2);
    return _lsm303_write_singular(hlsm, CTRL_REG6_A, ctrl_reg_6); 
}

HAL_StatusTypeDef _lsm303_boot_pad2(struct lsm303 *hlsm, bool en)
{
    HAL_StatusTypeDef status;
    uint8_t ctrl_reg_6;
    status = _lsm303_read_singular(hlsm, CTRL_REG6_A, &ctrl_reg_6);
    if (status != HAL_OK)
        return status;
    ctrl_reg_6 &= ~(1 << BOOT_I1);
    ctrl_reg_6 += (en << BOOT_I1);
    return _lsm303_write_singular(hlsm, CTRL_REG6_A, ctrl_reg_6); 
}

HAL_StatusTypeDef _lsm303_p2_active_function_status(struct lsm303 *hlsm, bool en)
{
    HAL_StatusTypeDef status;
    uint8_t ctrl_reg_6;
    status = _lsm303_read_singular(hlsm, CTRL_REG6_A, &ctrl_reg_6);
    if (status != HAL_OK)
        return status;
    ctrl_reg_6 &= ~(1 << P2_ACT);
    ctrl_reg_6 += (en << P2_ACT);
    return _lsm303_write_singular(hlsm, CTRL_REG6_A, ctrl_reg_6); 
}

HAL_StatusTypeDef _lsm303_interrupt_active(struct lsm303 *hlsm, enum LSM303_INTERRUPT_ACTIVE active)
{
    HAL_StatusTypeDef status;
    uint8_t ctrl_reg_6;
    status = _lsm303_read_singular(hlsm, CTRL_REG6_A, &ctrl_reg_6);
    if (status != HAL_OK)
        return status;
    ctrl_reg_6 &= ~(1 << H_LACTIVE);
    ctrl_reg_6 += (active << H_LACTIVE);
    return _lsm303_write_singular(hlsm, CTRL_REG6_A, ctrl_reg_6); 
}

HAL_StatusTypeDef _lsm303_reference_write(struct lsm303 *hlsm, uint8_t ref)
{
    return _lsm303_write_singular(hlsm, REFERENCE_A, ref);
}

HAL_StatusTypeDef _lsm303_reference_read(struct lsm303 *hlsm, uint8_t *pref)
{
    return _lsm303_read_singular(hlsm, REFERENCE_A, pref);
}

HAL_StatusTypeDef _lsm303_status_read(struct lsm303 *hlsm, uint8_t *pstatus)
{
    return _lsm303_read_singular(hlsm, STATUS_REG_A, pstatus);
}

HAL_StatusTypeDef _lsm303_accel_data(struct lsm303 *hlsm, uint8_t *pdata)
{
    return _lsm303_read_mult(hlsm, OUT_X_L_A, 6, pdata);
}
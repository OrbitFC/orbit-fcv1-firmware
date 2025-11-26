#include <misc/at24.h>

#ifndef MIN
    #define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
void at24_write_byte(struct at24 *hat, uint16_t addr, uint8_t val)
{
    uint8_t dev_addr = (uint8_t)DEVADDR | (hat->addr) | ((addr >> 8) & 0x1);
    struct i2cRequest ri2c;
    ri2c.type = I2C_REQUEST_TYPE_WRITE;
    ri2c.task = xTaskGetCurrentTaskHandle();
    ri2c.data = &val;
    ri2c.size = 1;
    ri2c.mem_addr_size = I2C_MEMADD_SIZE_8BIT;
    ri2c.dev_addr = dev_addr;
    ri2c.mem_addr = (uint8_t)addr;
    i2c_request(&ri2c);
}

void at24_write_page(struct at24* hat, uint16_t addr, uint8_t *val, uint16_t len)
{
    uint8_t dev_addr = (uint8_t)DEVADDR | (hat->addr) | ((addr >> 8) & 0x1);
    struct i2cRequest ri2c;
    ri2c.type = I2C_REQUEST_TYPE_WRITE;
    ri2c.task = xTaskGetCurrentTaskHandle();
    ri2c.data = val;
    ri2c.size = MIN(len, 16);
    ri2c.mem_addr_size = I2C_MEMADD_SIZE_8BIT;
    ri2c.dev_addr = dev_addr;
    ri2c.mem_addr = (uint8_t)addr;
    i2c_request(&ri2c);
}

void at24_read_current_addr(struct at24 *hat, uint16_t addr, uint8_t *pval)
{
    uint8_t dev_addr = (uint8_t)DEVADDR | (hat->addr) | ((addr >> 8) & 0x1);
    struct i2cRequest ri2c;
    ri2c.type = I2C_REQUEST_TYPE_MASTER_RECEIVE;
    ri2c.task = xTaskGetCurrentTaskHandle();
    ri2c.data = pval;
    ri2c.size = 1;
    ri2c.mem_addr_size = I2C_MEMADD_SIZE_8BIT;
    ri2c.dev_addr = dev_addr;
    ri2c.mem_addr = (uint8_t)addr;
    i2c_request(&ri2c);
}

void at24_read_current_addr_seq(struct at24 *hat, uint16_t addr, uint8_t *pval, uint16_t len)
{
    uint8_t dev_addr = (uint8_t)DEVADDR | (hat->addr) | ((addr >> 8) & 0x1);
    struct i2cRequest ri2c;
    ri2c.type = I2C_REQUEST_TYPE_MASTER_RECEIVE;
    ri2c.task = xTaskGetCurrentTaskHandle();
    ri2c.data = pval;
    ri2c.size = len;
    ri2c.mem_addr_size = I2C_MEMADD_SIZE_8BIT;
    ri2c.dev_addr = dev_addr;
    ri2c.mem_addr = (uint8_t)addr;
    i2c_request(&ri2c);
}

void at24_read_byte(struct at24 *hat, uint16_t addr, uint8_t *pval)
{
    uint8_t dev_addr = (uint8_t)DEVADDR | (hat->addr) | ((addr >> 8) & 0x1);
    struct i2cRequest ri2c;
    ri2c.type = I2C_REQUEST_TYPE_READ;
    ri2c.task = xTaskGetCurrentTaskHandle();
    ri2c.data = pval;
    ri2c.size = 1;
    ri2c.mem_addr_size = I2C_MEMADD_SIZE_8BIT;
    ri2c.dev_addr = dev_addr;
    ri2c.mem_addr = (uint8_t)addr;
    i2c_request(&ri2c);
}

void at24_read_byte_seq(struct at24 *hat, uint16_t addr, uint8_t *pval, uint16_t len)
{
    uint8_t dev_addr = (uint8_t)DEVADDR | (hat->addr) | ((addr >> 8) & 0x1);
    struct i2cRequest ri2c;
    ri2c.type = I2C_REQUEST_TYPE_READ;
    ri2c.task = xTaskGetCurrentTaskHandle();
    ri2c.data = pval;
    ri2c.size = len;
    ri2c.mem_addr_size = I2C_MEMADD_SIZE_8BIT;
    ri2c.dev_addr = dev_addr;
    ri2c.mem_addr = (uint8_t)addr;
    i2c_request(&ri2c);
}
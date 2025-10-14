#ifndef __AT24_H__
#define __AT24_H__
#include <stdint.h>

#define DEVADDR 0xA0

void at24_write_byte(uint16_t addr, uint8_t val);
void at24_write_page(uint16_t addr, uint8_t *val, uint16_t len);
void at24_read_current_addr(uint16_t addr, uint8_t *pval);
void at24_read_current_addr_seq(uint16_t addr, uint8_t *pval, uint16_t len);
void at24_read_byte(uint16_t addr, uint8_t *pval);
void at24_read_byte_seq(uint16_t addr, uint8_t *pval, uint16_t len);
#endif
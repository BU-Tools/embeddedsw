#ifndef FSBL_PROGRAMSI_H
#define FSBL_PROGRAMSI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "fsbl.h"
#include "xil_types.h"
#include "AXI_slave_addrs.h"
  /*serv base*/
#define SI_CONFIG_BASE_ADDR AXI_ADDR_SERV
  /*i2c base*/
#define SI_I2C_BASE_ADDR AXI_ADDR_SI
void SiI2cWrite(u8 address,u8 data);


void ProgramSI();

#ifdef __cplusplus
}
#endif

#endif  /* FSBL_PROGRAMSI_H */

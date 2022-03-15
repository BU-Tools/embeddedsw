#ifndef XFSBL_PROGRAMSI_H
#define XFSBL_PROGRAMSI_H

#define PRINTF fsbl_printf

#include "xfsbl_hw.h"
#include "xil_types.h"


#ifdef __cplusplus
extern "C" {
#endif


  //Code for writing to an si chip via i2c
  void SiI2cWrite(u32 axi_base_address, u8 i2c_address,u8 address,u8 data);

  //struct for data used to program an SI chip
  typedef struct {
    u32 i2c_AXIBaseAddr;         //Base address of the AXI i2c IP
    u32 si_enable_AXIAddr;       //Address used to enable the SI chip
    u32 si_enable_MASK;          //Mask of bits used to enable the SI chip
    u32 si_oe_AXIAddr;           //Address used to enable the SI outputs
    u32 si_oe_MASK;              //Mask of bits used to enable the SI output
    u32 si_locked_AXIAddr;       //Address used to check if the SI is locked (0 means don't check)
    u32 si_locked_MASK;          //Mask of bits used to tell if the SI is locked
    u16 const * data;            //Data used to config the Si Chip
    u32 data_init_size;          //number of entrys of config Data to use for init
    u32 data_size;               //Total number of entries in config Data
    u32 data_init_usleep;        //uS to sleep after the init data has been written
    char const * name;           //Name of Si chip being programmed
    u8  i2c_address;             // address of the si chip's i2c interface
  } sProgramSI;

  //call for programming a full SI chip
  void ProgramSI(sProgramSI config);





#ifdef __cplusplus
}
#endif

#endif  /* XFSBL_PROGRAMSI_H */

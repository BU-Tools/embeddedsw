#include "xfsbl_programSI.h"

void SiI2cWrite(u32 axi_base_address, u8 i2c_address, u8 address,u8 data){
  Xil_Out32((UINTPTR)(axi_base_address + OFFSET_I2C_RESET  )  ,(u32)(0xA));
  Xil_Out32((UINTPTR)(axi_base_address + OFFSET_I2C_CONTROL)  ,(u32)(0x2));
  Xil_Out32((UINTPTR)(axi_base_address + OFFSET_I2C_CONTROL)  ,(u32)(0xC));
  Xil_Out32((UINTPTR)(axi_base_address + OFFSET_I2C_TX_FIFO)  ,(u32)(0x100 | ((u32)i2c_address)));
  Xil_Out32((UINTPTR)(axi_base_address + OFFSET_I2C_TX_FIFO)  ,(u32)((u32) address));
  Xil_Out32((UINTPTR)(axi_base_address + OFFSET_I2C_TX_FIFO)  ,(u32)(0x200 | ((u32)data)));
  Xil_Out32((UINTPTR)(axi_base_address + OFFSET_I2C_CONTROL)  ,(u32)( 0xD));
  u16 tries = 20000;
  //Time out of something isn't working
  //  while(!(Xil_In32(I2C_STATUS) & 0x80)){
  while( (Xil_In32( ( axi_base_address+OFFSET_I2C_STATUS ) ) & 0x4 ) ){
    tries--;
    if(!tries){
      PRINTF(DEBUG_GENERAL,"Timeout on 0x%02X @ 0x%02X\r\n",data,address);
      break;
    }
  }
  //  usleep(50);
  Xil_Out32((axi_base_address + OFFSET_I2C_CONTROL), 0x0);

  //  PRINTF(DEBUG_GENERAL,"I2C Write: 0x%02X @ 0x%02X\r\n",data,address);
}


void ProgramSI(sProgramSi config){
  /*https://www.beyond-circuits.com/wordpress/2018/05/updating-the-first-stage-bootloader-in-petalinux-v2017-4/*/
  PRINTF(DEBUG_GENERAL,"\r\n\r\n\r\n\r\n\r\n");
  PRINTF(DEBUG_GENERAL,"========================================\r\n");
  PRINTF(DEBUG_GENERAL,"Programming %s \r\n",
	 config.name);
  PRINTF(DEBUG_GENERAL,"AXI I2C   @ 0x%08X \r\n",
	 config.i2c_AXIBaseAddr);//SI_I2C_BASE_ADDR);
  PRINTF(DEBUG_GENERAL,"Si enable        @ 0x%08X(0x%08X) Val: 0x%08X \r\n",
	 config.si_enable_AXIAddr,
	 config.si_enable_MASK,
	 Xil_In32(config.si_enable_AXIAddr) & config.si_enable_MASK);//SI_CONFIG_BASE_ADDR);
  PRINTF(DEBUG_GENERAL,"Si output enable @ 0x%08X(0x%08X) Val: 0x%08X \r\n",
	 config.si_oe_AXIAddr,
	 config.si_oe_MASK,
	 Xil_In32(config.si_oe_AXIAddr) & config.si_oe_MASK);//SI_CONFIG_BASE_ADDR);
  PRINTF(DEBUG_GENERAL,"Si locked        @ 0x%08X(0x%08X) Val: 0x%08X \r\n",
	 config.si_locked_AXIAddr,
	 config.si_locked_MASK,
	 Xil_In32(config.si_locked_AXIAddr) & config.si_locked_MASK);//SI_CONFIG_BASE_ADDR);
  
  
  //Disable the output of the SI chip
  if(config.si_oe_AXIAddr != 0){
    Xil_Out32(config.si_oe_AXIAddr,
	      (Xil_In32(config.si_oe_AXIAddr) & (~config.si_oe_MASK)));
  }
  //Enable the chip
  if(config.si_enable_AXIAddr != 0){
    Xil_Out32(config.si_enable_AXIAddr,
	      (Xil_In32(config.si_enable_AXIAddr) | (config.si_enable_MASK)));
  }
  
  u16 iWrite=0;
  //write the first config.configDataInitSize i2c writes to reset the chip (usually 5)
  for(; iWrite < config.data_init_size ;iWrite++){
    SiI2cWrite(config.i2c_AXIBaseAddr,
	       config.i2c_address,
	       (u8)((config.data[iWrite] >> 8) & 0xFF)  ,
	       (u8)((config.data[iWrite]     ) & 0xFF));
  }
  //Wait for reset to finish
  usleep(config.data_init_usleep);//usuall 400000);
  //Program the SI chip
  for(; iWrite < config.data_size;iWrite++){
    SiI2cWrite(config.i2c_AXIBaseAddr,
	       config.i2c_address,
	       (u8)((config.data[iWrite] >> 8) & 0xFF)  ,
	       (u8)((config.data[iWrite]     ) & 0xFF));
  }
  PRINTF(DEBUG_GENERAL,"Waiting for lock\r\n");
  
  //enable the output of the SI chip
  if(config.si_oe_AXIAddr != 0){
    Xil_Out32(config.si_oe_AXIAddr,
	      (Xil_In32(config.si_oe_AXIAddr) | (config.si_oe_MASK)));
    usleep(1000000); //wait 1s
  }

  
  u16 tries = 20000;
  //Waitinf for lock
  if(config.si_locked_AXIAddr != 0){
    while((Xil_In32(config.si_locked_AXIAddr) & config.si_locked_MASK)){ //usually 6
      usleep(1000);
      tries--;
      if(!tries){
	PRINTF(DEBUG_GENERAL,"Warning: Timeout waiting for Si lock\r\n");
	break;
      }
    }
  }
  
  PRINTF(DEBUG_GENERAL,"========================================\r\n");
  PRINTF(DEBUG_GENERAL,"\r\n\r\n\r\n\r\n\r\n");
  
}

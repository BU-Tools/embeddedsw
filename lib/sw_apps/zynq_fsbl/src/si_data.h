#ifndef __SI_DATA__H_
#define __SI_DATA__H_

#ifdef __cplusplus
extern "C" {
#endif

#include "xil_types.h"
#include "fsbl_programSI.h"

  void siConfigInit();
  u32 siConfigCount;
  sProgramSI * siConfigs;

#ifdef __cplusplus
}
#endif

#endif

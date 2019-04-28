#ifndef __F_VL53L1X_H
#define __F_VL53L1X_H

#include "main.h"

#define VL53L1X_AVANT_1_ADDR	0x10
#define VL53L1X_AVANT_2_ADDR	0x12
#define VL53L1X_AVANT_3_ADDR	0x14
#define VL53L1X_ARRIERE_1_ADDR	0x16



void F_VL53L1X_InitSensors();
void F_VL53L1X_CheckSensors();

#endif

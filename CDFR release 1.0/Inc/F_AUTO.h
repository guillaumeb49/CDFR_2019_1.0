/*
 * F_Regulation.h
 *
 *  Created on: Sep 13, 2016
 *      Author: mario
 */

#ifndef F_AUTO_H_
#define F_AUTO_H_

#include "main.h"
#include <stdint.h>

/* #### Flags I2C #### */
#define STATUS_AUTO_OK		0
#define STATUS_AUTO_ERROR	1

/* #### Paramètres FSM régulation #### */
#define NB_POINT_MAX 	254

#define WAIT_LOOP		6
#define NEXT_POINT		1
#define REG_CAP_DIST	2
#define REG_TETA		3
#define REG_TARGET_OK	4

/* #### Paramètres régulateur #### */
#define KP_SPEED 		0.1	// Speed regulation
#define KI_SPEED		0.04
#define KD_SPEED		0

#define KP_TETA_POS 	200		// Angular regulation
#define KI_TETA_POS 	0
#define KD_TETA_POS 	0

#define KP_DIST_POS 	1		// Position regulator
#define KI_DIST_POS 	0

#define DV_MAX			10		// Acceleration max
#define V_MAX			200.0	// Vit max en mm/s

/* #### Fonctions principales #### */
void  F_AUTO_SpeedRegulator(void);
void  F_AUTO_PositionRegulator(void);
void  F_AUTO_AngularPositionRegulator(void);

/* #### Interfaces #### */
uint8_t  F_AUTO_AddTargetLocalisation(Localisation );
uint8_t  F_AUTO_AddTargetPoint(float x_mm , float y_mm, float angle_deg);
void F_AUTO_Enable(void);
void F_AUTO_Disable(void);
void F_AUTO_ResetTargetList(void);
void F_AUTO_setSpeed(float leftSpeed, float rightSpeed);

/* #### Outils #### */
void  F_AUTO_Normalise(float *cmd_L ,float *cmd_R, float max);
float F_AUTO_DistanceAvantCible(void);
float F_AUTO_DistanceAvantPoint(void);

#endif /* F_AUTO_H_ */

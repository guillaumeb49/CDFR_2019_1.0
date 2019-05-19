/*
 * drv_QEI.h
 *
 *  Created on:  20/07/2016
 *      Author: mario
 */

#ifndef D_QEI_H_
#define D_QEI_H_

#include "main.h"
#include "D_LCD.h"

// Paramètres mécaniques

#define ENTRAXE 	240	// Interval between wheels in millimeter
#define WHEEL_D		72
#define WHEEL_R		WHEEL_D/2
#define QEI_RES		2400	// 600*4 = Resolution par tour x nb de front
#define RBT_OF7_X 	0
#define RBT_OF7_Y 	0

// Paramètres internes
#define TICKTOMM 	PI*WHEEL_D/QEI_RES	// Move from tick to millimeter
#define FREQ_ECH 	50			 		// Update frequency

// #### Définition des fonctions ####
void Init_QEI(void);
void F_QEI_Reset(void);
void F_QEI_Read (void);
void F_QEI_XYTetaProcessing(void);
// ## Interfaces ##
void F_QEI_setLocalisation( Localisation );
void F_QEI_setPosition( int x_mm, int y_mm, int teta_deg);
void F_QEI_getspeed(float * , float * );
void F_QEI_getEstimatedState(Localisation * state);
void F_QEI_printEstimatedState(void);
void F_QEI_printCounters(void);
void F_QEI_printCountersLCD(void);
void F_QEI_printEstimatedSpeed(void);

#endif /* DRV_QEI_ */

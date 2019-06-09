/*
 * F_Regulation.c
 *
 *  Created on: Sep 13, 2016
 *      Author: mario
 */

#include "F_AUTO.h"

int g_enableAUTO=0;
int g_obstacle_not=0;

/* #### Variables speed regulator #### */
float g_cons_speed_right =0 ;
float g_cons_speed_left =0 ;
float g_cmd_right_old =0;
float g_cmd_left_old =0;
//float g_direction=1;
// Sum error for each wheel
float g_errRightSum =0;
float g_errLeftSum =0;
// Old error value for each wheel
float g_errRightOld =0;
float g_errLeftOld =0;

/* #### Variables positions regulator #### */
float g_tetaSum  = 0;
float g_errT_old = 0;
float g_distSum  = 0;

extern Localisation g_estimate;

int g_i_point=0;
int g_nbPoints=0;
int g_targetPositionFlag=0;
int g_targetAngularFlag=0;

int token=0;
Localisation consigne[NB_POINT_MAX];

/*
 * Speed regulator for each wheel
 */
void F_AUTO_SpeedRegulator(void){
	/* ### Variables locales ### */
	// Données d'entrée
	float l_rightSpeed;
	float l_leftSpeed;

	// Variables du régulateur
	float l_errRigth;
	float l_errLeft;
	float l_errRigthDt;
	float l_errLeftDt;
	float l_cmd_right_dV;
	float l_cmd_left_dV;

	// Variables de sortie
	float l_cmd_right;
	float l_cmd_left;

	// Données d'entée de régulateur
	F_QEI_getspeed(&l_leftSpeed,&l_rightSpeed);

	// Calcul des erreurs
	l_errLeft  = g_cons_speed_left  - l_leftSpeed;
	l_errRigth = g_cons_speed_right - l_rightSpeed;

	l_errLeftDt  = l_errLeft - g_errLeftOld ;
	l_errRigthDt = l_errRigth - g_errRightOld ;

	if(token==REG_TETA)
	{
		g_errLeftSum  += l_errLeft  * g_enableAUTO;
		g_errRightSum += l_errRigth * g_enableAUTO;
	}
	else
	{
		g_errLeftSum  += l_errLeft  * g_enableAUTO * g_obstacle_not;
		g_errRightSum += l_errRigth * g_enableAUTO * g_obstacle_not;
	}
	g_errLeftOld  = l_errLeft;
	g_errRightOld = l_errRigth;

	// Calcul de la commande

	l_cmd_left  = (l_errLeft  * KP_SPEED) + (g_errLeftSum  * KI_SPEED) + (l_errLeftDt  * KD_SPEED);
	l_cmd_right = (l_errRigth * KP_SPEED) + (g_errRightSum * KI_SPEED) + (l_errRigthDt * KD_SPEED);

	// Gestion de l'acceleration
	l_cmd_right_dV = l_cmd_right - g_cmd_right_old ;
	l_cmd_left_dV  = l_cmd_left  - g_cmd_left_old  ;
	g_cmd_left_old  = l_cmd_left ;
	g_cmd_right_old = l_cmd_right;
	if(l_cmd_right_dV > DV_MAX) l_cmd_right += DV_MAX ;
	if(l_cmd_left_dV  > DV_MAX) l_cmd_left  += DV_MAX ;

	// Application de la commande
	if((g_enableAUTO==0)|| (g_obstacle_not==0 && token!=REG_TETA) ){
		F_PWM_SetCmdMotorGauche( 0 );
		F_PWM_SetCmdMotorDroit( 0 );
	}else{
		F_PWM_SetCmdMotorGauche((int)l_cmd_left);
		F_PWM_SetCmdMotorDroit((int)l_cmd_right);
	}
}
/*
 * Angular regulator called in Timer 10 interrupt
 */
void F_AUTO_AngularPositionRegulator(void){
	// Variables locale
	float l_errT;
	float l_errT_D;

	// Errors processing
	l_errT = consigne[g_i_point].teta - g_estimate.teta;
	F_Math_ModuloPi(&l_errT);

	l_errT_D = l_errT - g_errT_old;
	g_tetaSum += l_errT * g_enableAUTO;

	g_errT_old=l_errT;

	// Command processing
	g_cons_speed_right =  ((l_errT * KP_TETA_POS) + (g_tetaSum * KI_TETA_POS) + (l_errT_D * KD_TETA_POS)) ;
	g_cons_speed_left  = -g_cons_speed_right;

	F_AUTO_Normalise(&g_cons_speed_right, &g_cons_speed_left,V_MAX);
}
/*
 * Position regulator called in Timer 10 interrupt
 */
void F_AUTO_PositionRegulator(void){
	// Variables local
	float l_errT;		// Angular error
	float l_errT_D;		// Angular error evolution
	float l_errD=0;		// Distance error
	float l_angularReg;	// Sum of proportional, derivative and integral terms of angular error
	float l_distanceReg;// Sum of proportional, derivative and integral terms of distance error
	float l_cap;		// Cape needed to join the following point
	float l_ralentissement=0;

	switch(token){
		case WAIT_LOOP: // Attente de l'activation de g_enableAuto
			if(g_enableAUTO==1 && g_nbPoints!=0){
				token=REG_CAP_DIST;
			}else{
				g_cons_speed_right = 0 ;
				g_cons_speed_left  = 0 ;
			}
			break;
		case NEXT_POINT: // Passage au point suivant

			if(g_i_point == (g_nbPoints-1) ){	// j'ai atteint le dernier point
				printf("AUTO__Pos OK \n\r");
				g_tetaSum=0;
				token = REG_TETA;
			}else{
				if(g_enableAUTO) g_i_point ++ ;				// Je passe au point suivant
				if(g_i_point !=0) printf("AUTO__Idx : %d NbPts : %d\n\r",g_i_point,g_nbPoints);
				token = REG_CAP_DIST;
			}

			if(g_enableAUTO==0) token = WAIT_LOOP;
			break;

		case REG_CAP_DIST: // Regulation en cap et avance

			// #### Est-ce que je suis arrivé ? ####
			if( F_AUTO_DistanceAvantPoint() < 10 ){
				g_tetaSum=0;
				g_errT_old=0;
				g_distSum=0;

				token = NEXT_POINT;
				break;
			}
			// #### Je ne suis pas arrivé ####

			// Gestion de l'angle
			l_cap = atan2f(consigne[g_i_point].y - g_estimate.y , consigne[g_i_point].x - g_estimate.x);
			l_errT = l_cap - g_estimate.teta ;	// Cape error calculating
			F_Math_ModuloPi(&l_errT) ;

			l_errT_D   = l_errT - g_errT_old;		// Derivative

			if(token==REG_TETA)
			{
				g_tetaSum += l_errT * g_enableAUTO;		// Update sum
			}
			else
			{
				g_tetaSum += l_errT * g_enableAUTO * g_obstacle_not;		// Update sum
			}

			g_errT_old = l_errT;					// Save value

			// Gestion de la distance
			l_ralentissement = ( F_Math_Abs(l_errT)/PI ) * V_MAX * 8 ;
			l_errD = V_MAX;//KP_DIST_POS * F_AUTO_DistanceAvantCible() ;

			if( l_errD > V_MAX){
				l_errD = V_MAX;
			}
			if(l_ralentissement > l_errD){
				l_ralentissement = l_errD ;
			}

			l_errD = l_errD - l_ralentissement;
			//g_distSum += l_errD * g_enableAUTO;		// Update sum

			// Calcul de la commande
			l_angularReg  = ( KP_TETA_POS * l_errT + KI_TETA_POS * g_tetaSum + KD_TETA_POS * l_errT_D ) ;
			l_distanceReg = ( KP_DIST_POS * l_errD );//+ KI_DIST_POS * g_distSum) ;

			g_cons_speed_right = l_distanceReg + l_angularReg ;
			g_cons_speed_left  = l_distanceReg - l_angularReg ;

			if(g_enableAUTO==0) token = WAIT_LOOP;
			break;

		case REG_TETA: // Regulation en teta

			l_errT =   consigne[g_i_point].teta - g_estimate.teta ;	// Theta error calculating
			F_Math_ModuloPi(&l_errT);

			if(F_Math_Abs(l_errT) < F_Math_toRad(1)){
				printf("AUTO__Teta OK \n\r");
				token = REG_TARGET_OK;
				break;
			}

			l_errT_D   = l_errT - g_errT_old;		// Derivative
			if(token==REG_TETA)
			{
				g_tetaSum += l_errT * g_enableAUTO;		// Update sum
			}
			else
			{
				g_tetaSum += l_errT * g_enableAUTO * g_obstacle_not;		// Update sum
			}

			g_errT_old = l_errT;					// Save value

			// Calcul de la commande
			l_angularReg  = ( KP_TETA_POS * l_errT + KI_TETA_POS * g_tetaSum + KD_TETA_POS * l_errT_D ) ;

			g_cons_speed_right =  l_angularReg ;
			g_cons_speed_left  = -l_angularReg ;

			if(g_enableAUTO==0) token = WAIT_LOOP;
			break;

		case REG_TARGET_OK:// Je suis arrivé
			F_AUTO_ResetTargetList();
			g_i_point=0;
			token = WAIT_LOOP;
			break;
		default: // Error go to state 1;
			token = WAIT_LOOP;
	}

	F_AUTO_Normalise(&g_cons_speed_left,&g_cons_speed_right,V_MAX);

}
/*
 * Add point trajectory
 */
uint8_t F_AUTO_AddTargetLocalisation(Localisation loc){
	uint8_t returnCode;
	returnCode = F_AUTO_AddTargetPoint(loc.x,loc.y,loc.teta);
	return returnCode;
}
/*
 * Add point trajectory
 */
uint8_t F_AUTO_AddTargetPoint(float x_mm , float y_mm, float angle_deg){
		//y_mm= -y_mm;
	if(g_nbPoints<NB_POINT_MAX){
		angle_deg=F_Math_toRad(angle_deg);
		F_Math_ModuloPi(&angle_deg);

		consigne[g_nbPoints].x = x_mm;
		consigne[g_nbPoints].y = y_mm;
		consigne[g_nbPoints].teta = angle_deg;

		g_nbPoints++;
		printf("AUTO: AddPt: Nb de point = %d Dist tot = %d \n\r",g_nbPoints,(int)F_AUTO_DistanceAvantCible());
		return STATUS_AUTO_OK;
	}else{
		printf("AUTO: AddPt: Erreur nb de pts trop important \n\r");
		return STATUS_AUTO_ERROR;
	}
}
/*
 *
 */
void F_AUTO_ResetTargetList(void){
	g_nbPoints=0;
}
/*
 *
 */
float F_AUTO_DistanceAvantPoint(){
	float a = g_estimate.x - consigne[g_i_point].x;
	float b = g_estimate.y - consigne[g_i_point].y;
	float dist = sqrtf((a*a)+(b*b)) ;

	return dist;
}
/*
 * Distance between us and final target point
 */
float F_AUTO_DistanceAvantCible(){
	float a = g_estimate.x - consigne[g_i_point].x;
	float b = g_estimate.y - consigne[g_i_point].y;
	float dist = sqrtf((a*a)+(b*b)) ;
	int i;

	for( i=g_i_point; i<g_nbPoints-1 ; i++ ){
		a = consigne[i].x - consigne[i+1].x;
		b = consigne[i].y - consigne[i+1].y;
		dist += sqrtf((a*a)+(b*b)) ;
	}

	return dist;
}
/*
 * Return arg % pi
 */
void F_AUTO_Normalise(float *cmd_L , float *cmd_R, float max){
	float x;
	float cmd_right = *cmd_R ;
	float cmd_left  = *cmd_L ;

	if(F_Math_Abs(cmd_right) > max || F_Math_Abs(cmd_left) > max){
		if(F_Math_Abs(cmd_right) > max && F_Math_Abs(cmd_right) > F_Math_Abs(cmd_left) ){
			x = F_Math_Sign(cmd_right) * max / cmd_right ;
		}else{
			x = F_Math_Sign(cmd_left) * max / cmd_left ;
		}

		*cmd_R = cmd_right * x;
		*cmd_L  = cmd_left  * x;
	}
}
/*
 * Enable automatic loop
 */
void F_AUTO_Enable(void){
	g_enableAUTO = 1 ;
}
/*
 * Disable automatic loop
 */
void F_AUTO_Disable(void){
	g_enableAUTO = 0 ;
}
/*
 * Consigne de vitesse
 */
void F_AUTO_setSpeed(float leftSpeed, float rightSpeed){
	g_cons_speed_left  = leftSpeed ;
	g_cons_speed_right = rightSpeed ;

	F_AUTO_Normalise(&g_cons_speed_left, &g_cons_speed_right,V_MAX);
}

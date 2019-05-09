/*
 * A_TCP.h
 *
 *  Created on: 5 août 2018
 *      Author: guillaume
 */

#ifndef A_TCP_H_
#define A_TCP_H_

#include "tcp_server.h"

#define NB_OCTETS_CMD 15
#define SIZE_PARAM_CMD 4

/* structure that represents a command between the Raspberry Pi and the STM32*/
typedef struct tcp_command
{
	uint32_t id;
	uint8_t nb_octet;
	uint8_t cmd;
	uint8_t nb_param;
	uint16_t params[SIZE_PARAM_CMD];
}tcp_command;




/* structure that represents an answer between the STM32 and the Raspberry Pi*/
typedef struct tcp_answer
{
	uint32_t id;
	uint8_t nb_octet;
	uint8_t cmd;
	uint8_t code_retour;
	uint16_t reponse[SIZE_PARAM_CMD];
}tcp_answer;

void F_TCP_paquetTocmd(struct pbuf *p, struct tcp_command *s_cmd_received);
void F_TCP_answerTotab(uint8_t *array, struct tcp_answer s_cmd_answer);
void F_TCP_cmdTotab(uint8_t *array, struct tcp_command s_cmd);
uint8_t F_TCP_send_command(struct tcp_command s_cmd);

#endif /* A_TCP_H_ */

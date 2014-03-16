/*
 * ipc.h
 *
 *  Created on: 16.03.2014
 *      Author: phil
 */

#ifndef IPC_H_
#define IPC_H_

#include "Message/Message.h"

#define NOOPERATION			0
#define SEND 				1
#define RECEIVE 			2
#define SENDRECEIVE			3
#define RECEIVE_ASYNC		6

#define OK					 0
#define OTHER_NOT_FOUND		-1
#define DEAD				-2
#define NOTHING_RECEIVED	-3

uint32_t syscall(ProcessID dst, uint32_t callingtype, Message* msg);

#endif /* IPC_H_ */

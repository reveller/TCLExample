/*
 * Command.h
 *
 *  Created on: Oct 1, 2012
 *      Author: Zephram
 */

#ifndef COMMAND_H_
#define COMMAND_H_
#include "arduino.h"



class Command {
public:
	Command();
	~Command();
	void Execute();
	//Send Info
	void DoorOpen(void);
	void DoorClosed(void);
	void SendTemps(float,float);
private:
	//respond
	void initializeProfile(ComMessage*);
	void sendAtmosphereTemp(ComMessage*);
	void sendLiquidTemp(ComMessage*);
	void sendIsAlive(ComMessage*);
	void sendTemps(ComMessage*);
	void sendProfilePosition(ComMessage*);
	void synchTime(ComMessage*);
	void sendState(ComMessage*);
	void sendStatus(ComMessage*);
	void sendAll(ComMessage*);
	void setClock(ComMessage*);
	char currentCommand[300];
	long CommandId;
};
#endif
/* COMMAND_H_ */

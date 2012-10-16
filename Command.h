/*
 * Command.h
 *
 *  Created on: Oct 1, 2012
 *      Author: Zephram
 */

#ifndef COMMAND_H_
#define COMMAND_H_
#include "arduino.h"
#include "ComMessage.h"
#include "Time.h"
#include "TempControl.h"


class Command {
public:
	Command(TempControl*);
	~Command();
	void Execute();
	//Send Info
//	void DoorOpen(void);
//	void DoorClosed(void);
//	void SendTemps(void);
//	void SendError(char*);
//	void SendAll(void);
private:
	//respond
//	void initializeProfile(ComMessage*);
//	void sendAtmosphereTemp(ComMessage*);
//	void sendLiquidTemp(ComMessage*);
//	void sendIsAlive(ComMessage*);
//	void sendTemps(ComMessage*);
//	void sendProfilePosition(ComMessage*);
//	void synchTime(ComMessage*);
//	void sendState(ComMessage*);
//	void sendStatus(ComMessage*);
//	void sendAll(ComMessage*);
//	void setClock(ComMessage*);
	char currentCommand[300];
	long CommandId;
	int commandIndex;
	TempControl *_TempControl;
};
#endif
/* COMMAND_H_ */

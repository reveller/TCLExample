/*
 * Command.cpp
 *
 *  Created on: Oct 1, 2012
 *      Author: Zephram
 */
#include "Command.h"



Command::Command()
{
	CommandId=0;

}
Command::~Command()
{


}

void Command::Execute()
{
	int index=0;
	while(Serial.available()>0 && index<299){

		currentCommand[index] = Serial.read();
		if(currentCommand[index]!='/0'){
			index++;
		}
		else {
			ComMessage *msg = ComMessage(currentCommand);
			//Evaluate read to find the correct method to call.
			switch(msg->CmdHash)
			{
				case 3582820445://InitializeProfile
					this->initializeProfile(msg);
					break;
				case 3866114098: //GetAtmosphereTemp
					this->sendAtmosphereTemp(msg);
					break;
				case 2977884258://GetLiquidTemp
					this->sendLiquidTemp(msg);
					break;
				case 1291098995://GetIsAlive
					this->sendIsAlive(msg);
					break;
				case 3094295831: //GetTemps
					this->sendTemps(msg);
					break;
				case 1725362106://GetProfilePosition
					this->sendProfilePosition(msg);
					break;
				case 4258256476://GetStatus
					this->sendStatus(msg);
					break;
				case 1159368095://GetState
					this->sendState(msg);
					break;
				case 3228941447://GetAll
					this->sendAll(msg);
					break;
				case 3663052520:
					this->setClock(msg);
					break;
			}
			delete msg;
			index++;
		}
	}
}

void Command::DoorOpen(void)
{
	CommandId++;
	ComMessage *msg = ComMessage(CommandId,"DoorOpen","door=open");
	Serial.println(msg->SerialMessage());
	delete msg;
}

void Command::DoorClosed()
{
	CommandId++;
	ComMessage *msg = ComMessage(CommandId,"DoorClossed","door=closed");
	Serial.println(msg->SerialMessage());
	delete msg;
}

void Command::SendTemps(float atmosTemp,float liquidTemp)
{
	CommandId++;
	char chrAll[100];
	sprintf(chrAll,"atmosphere=%n,liquid=%n",atmosTemp,liquidTemp);
	ComMessage *msg = ComMessage(CommandId,"SendTemps",chrAll);
	Serial.println(msg->SerialMessage());
	delete msg;
}

void Command::SendAll(float atmosTemp,float liquidTemp,char* status,char* state)
{
	CommandId++;
	char chrAll[200];
	sprintf(chrAll,"atmosphere=%n,liquid=%n,status=%s,state=%s",atmosTemp,liquidTemp,status,state);
	ComMessage *msg = ComMessage(CommandId,"SendTemps",chrAll);
	Serial.println(msg->SerialMessage());
	delete msg;
}


void Command::initializeProfile(ComMessage* msg)
{
	//Save off the profile;
//	Serial.print("");
//	Serial.println("");
}

void Command::setClock(ComMessage* msg)
{
	//parse out the clock epoch
	char* parsed = strtok(msg->Params,"=");
	parsed = strtok(NULL,"=");
	while(parsed)
	{
		if(strcmp("time",parsed))
		{
			parsed = strtok(NULL,"=");
			if(isDigit(parsed)) {
				long pctime = atol(parsed);
				setTime(pctime);
			}
		}
	}

	ComMessage *m = new ComMessage(msg->Id,msg->Cmd,"result=success");
	Serial.println(m->SerialMessageReply());
	delete m;
}

void Command::sendAtmosphereTemp(ComMessage* msg)
{
	long atmos = 0;
	char chrAtmo[30];
	sprintf(chrAtmo,"atmosphere=%n",atmos);
	ComMessage *m = new ComMessage(msg->Id,msg->Cmd,chrAtmo);
	Serial.println(m->SerialMessage());
	delete m;
}

void Command::sendStatus(ComMessage* msg)
{
	char chrStatus[30];
	sprintf(chrStatus,"status=%n",/*getstatus*/);
	ComMessage *m = new ComMessage(msg->Id,msg->Cmd,chrStatus);
	Serial.println(m->SerialMessageReply()());
	delete m;
}

void Command::sendState(ComMessage* msg)
{
	long atmos = 0;
	char chrState[30];
	sprintf(chrState,"state=%n",/*getstate*/);
	ComMessage *m = new ComMessage(msg->Id,msg->Cmd,chrState);
	Serial.println(m->SerialMessageReply());
	delete m;
}

void Command::sendAll(ComMessage* msg)
{
	long atmos = 0;
	long liq = 0;
	char chrState[20];
	char chrStatus[20];
	char profileLoc[20];
	char chrAll[100];
	sprintf(chrAll,"atmosphere=%n,liquid=%n,status=%s,state=%s",atmos,liq,chrStatus,chrState);
	ComMessage *m = new ComMessage(msg->Id,msg->Cmd,chrAll);
	Serial.println(m->SerialMessageReply());
	delete m;
}

void Command::sendLiquidTemp(ComMessage* msg)
{
	long liq = 0;
	char chrliq[30];
	sprintf(chrliq,"liquid=%n",liq);
	ComMessage *m = new ComMessage(msg->Id,msg->Cmd,chrliq);
	Serial.println(m->SerialMessageReply());
	delete m;
}
void Command::sendIsAlive(ComMessage* msg)
{
	ComMessage *m = new ComMessage(msg->Id,msg->Cmd,"isalive=Alive");
	Serial.println(m->SerialMessageReply());
	delete m;
}
void Command::sendTemps(ComMessage* msg)
{
	long atmos = 0;
	long liq = 0;
	char chrTemps[50];
	sprintf(chrTemps,"liquid=%n,atmosphere=%n",liq,atmos);
	ComMessage *m = new ComMessage(msg->Id,msg->Cmd,chrTemps);
	Serial.println(m->SerialMessageReply());
	delete m;
}
void Command::sendProfilePosition(ComMessage* msg)
{
	ComMessage *m = new ComMessage(msg->Id,msg->Cmd,"alive=alive");
	Serial.println(m->SerialMessageReply());
	delete m;
}

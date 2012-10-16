/*
 * Command.cpp
 *
 *  Created on: Oct 1, 2012
 *      Author: Zephram
 */
#include "Command.h"



Command::Command(TempControl *tempCtrl)
{
	CommandId = 0;
	commandIndex = 0;
	_TempControl = tempCtrl;
	Serial.println("Created");
}

Command::~Command()
{


}

void Command::Execute()
{
//	Serial.println("before");
	while(Serial.available()>0 && commandIndex<299){
		Serial.println("after");
		currentCommand[commandIndex] = Serial.read();
		if(currentCommand[commandIndex]!='\n'){
			Serial.print(currentCommand[commandIndex]);
			commandIndex++;
		}
		else {
			currentCommand[commandIndex] = '\0';
			Serial.print("Received: [");
			Serial.print(currentCommand);
			Serial.println(']');
			commandIndex = 0;
//			ComMessage *msg = new ComMessage(currentCommand);
//			delete msg;
		}
//			if (msg->isInit){
//				//Evaluate read to find the correct method to call.
//				switch(msg->CmdHash)
//				{
//					case 3582820445U://InitializeProfile
//						this->initializeProfile(msg);
//						break;
//					case 3866114098U: //GetAtmosphereTemp
//						this->sendAtmosphereTemp(msg);
//						break;
//					case 2977884258U://GetLiquidTemp
//						this->sendLiquidTemp(msg);
//						break;
//					case 1291098995U://GetIsAlive
//						this->sendIsAlive(msg);
//						break;
//					case 3094295831U: //GetTemps
//						this->sendTemps(msg);
//						break;
//					case 1725362106U://GetProfilePosition
//						this->sendProfilePosition(msg);
//						break;
//					case 4258256476U://GetStatus
//						this->sendStatus(msg);
//						break;
//					case 1159368095U://GetState
//						this->sendState(msg);
//						break;
//					case 3228941447U://GetAll
//						this->sendAll(msg);
//						break;
//					case 3663052520U:
//						this->setClock(msg);
//						break;
//				}
//			}
//			else
//				Serial.println("Bad Message");
//				this->SendError("Bad Message Header");
			//delete msg;
//		}
	}
}

//void Command::SendError(char* ErrorMsg)
//{
//	CommandId++;
//	ComMessage *msg = new ComMessage(CommandId,"Error",ErrorMsg);
//	Serial.println(msg->SerialMessage());
//	delete msg;
//}
//
//
//void Command::DoorOpen(void)
//{
//	CommandId++;
//	ComMessage *msg = new ComMessage(CommandId,"DoorOpen","door=open");
//	Serial.println(msg->SerialMessage());
//	delete msg;
//}
//
//void Command::DoorClosed()
//{
//	CommandId++;
//	ComMessage *msg = new ComMessage(CommandId,"DoorClossed","door=closed");
//	Serial.println(msg->SerialMessage());
//	delete msg;
//}
//
//void Command::SendTemps()
//{
//	CommandId++;
//	char chrAll[100];
//	sprintf(chrAll,"atmosphere=%f,liquid=%f",(double)_TempControl->GetFridgeTemp(),(double)_TempControl->GetBeerTemp());
//	ComMessage *msg = new ComMessage(CommandId,"SendTemps",chrAll);
//	Serial.println(msg->SerialMessage());
//	delete msg;
//}
//
//void Command::SendAll()
//{
//	CommandId++;
//	char chrAll[200];
//	sprintf(chrAll,"atmosphere=%f,liquid=%f,status=%s,state=%s",(double)_TempControl->GetFridgeTemp(),
//			(double)_TempControl->GetBeerTemp(),_TempControl->GetModeStr(),_TempControl->GetStateStr());
//	ComMessage *msg = new ComMessage(CommandId,"SendTemps",chrAll);
//	Serial.println(msg->SerialMessage());
//	delete msg;
//}
//
//
//void Command::initializeProfile(ComMessage* msg)
//{
//	//Save off the profile;
////	Serial.print("");
////	Serial.println("");
//}
//
//void Command::setClock(ComMessage* msg)
//{
//	//parse out the clock epoch
//	char* parsed = strtok(msg->Params,"=");
//	parsed = strtok(NULL,"=");
//	while(parsed)
//	{
//		if(strcmp("time",parsed))
//		{
//			parsed = strtok(NULL,"=");
//			if(isDigit(*parsed)) {
//				long pctime = atol(parsed);
//				setTime(pctime);
//			}
//		}
//	}
//
//	ComMessage *m = new ComMessage(msg->Id,msg->Cmd,"result=success");
//	Serial.println(m->SerialMessageReply());
//	delete m;
//}
//
//void Command::sendAtmosphereTemp(ComMessage* msg)
//{
//	float atmos = _TempControl->GetFridgeTemp();
//	char chrAtmo[30];
//	sprintf(chrAtmo,"atmosphere=%f",(double)atmos);
//	ComMessage *m = new ComMessage(msg->Id,msg->Cmd,chrAtmo);
//	Serial.println(m->SerialMessage());
//	delete m;
//}
//
//void Command::sendStatus(ComMessage* msg)
//{
//	char chrStatus[30];
//	sprintf(chrStatus,"status=%s",_TempControl->GetModeStr());
//	ComMessage *m = new ComMessage(msg->Id,msg->Cmd,chrStatus);
//	Serial.println(m->SerialMessageReply());
//	delete m;
//}
//
//void Command::sendState(ComMessage* msg)
//{
//	char chrState[30];
//	sprintf(chrState,"state=%s",_TempControl->GetStateStr());
//	ComMessage *m = new ComMessage(msg->Id,msg->Cmd,chrState);
//	Serial.println(m->SerialMessageReply());
//	delete m;
//}
//
//void Command::sendAll(ComMessage* msg)
//{
//
//	char profileLoc[20];
//	char chrAll[100];
//	sprintf(chrAll,"atmosphere=%f,liquid=%f,status=%s,state=%s",(double)_TempControl->GetFridgeTemp(),
//			(double)_TempControl->GetBeerTemp(),_TempControl->GetModeStr(),_TempControl->GetStateStr());
//	ComMessage *m = new ComMessage(msg->Id,msg->Cmd,chrAll);
//	Serial.println(m->SerialMessageReply());
//	delete m;
//}
//
//void Command::sendLiquidTemp(ComMessage* msg)
//{
//	char chrliq[30];
//	sprintf(chrliq,"liquid=%f",(double)_TempControl->GetBeerTemp());
//	ComMessage *m = new ComMessage(msg->Id,msg->Cmd,chrliq);
//	Serial.println(m->SerialMessageReply());
//	delete m;
//}
//void Command::sendIsAlive(ComMessage* msg)
//{
//	ComMessage *m = new ComMessage(msg->Id,msg->Cmd,"isalive=Alive");
//	Serial.println(m->SerialMessageReply());
//	delete m;
//}
//void Command::sendTemps(ComMessage* msg)
//{
//	char chrTemps[50];
//	sprintf(chrTemps,"liquid=%f,atmosphere=%f",(double)_TempControl->GetBeerTemp(),(double)_TempControl->GetFridgeTemp());
//	ComMessage *m = new ComMessage(msg->Id,msg->Cmd,chrTemps);
//	Serial.println(m->SerialMessageReply());
//	delete m;
//}
//void Command::sendProfilePosition(ComMessage* msg)
//{
//	ComMessage *m = new ComMessage(msg->Id,msg->Cmd,"alive=alive");
//	Serial.println(m->SerialMessageReply());
//	delete m;
//}

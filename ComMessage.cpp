/*
 * ComMessage.cpp
 *
 *  Created on: Oct 3, 2012
 *      Author: Zephram
 */

#include "ComMessage.h"

ComMessage::ComMessage(char * input){

	if((input[0]!='<') || (input == NULL)){
		Serial.println("input:");
		Serial.println(input);
		isInit = 0;
		Serial.println("afterward");
		Serial.println(isInit);
		Serial.println(false);
		Serial.println("Bad Command");
		Serial.println(isInit);
		return;
	}

//	Cmd = NULL;
//	Params = NULL;
	Id = 0;
	CmdHash = 0;

	Serial.println("Good Command!");
	char* parsed = strtok(input,"<->");
	if (parsed != NULL){
		Serial.println(parsed);
		Serial.println("Parsed1");
	}
	parsed = strtok(NULL,"<->");
	if (parsed != NULL){
		Serial.println(parsed);
		Serial.println("Parsed2");
	}
	parsed = strtok(NULL,"<->");
	if (parsed != NULL){
		Serial.println(parsed);
		Serial.println("Parsed3");
	}

//
//	int paramNum = 0;
//
//	while (parsed)
//	{
//		switch(paramNum)
//		{
//		case 0:
//			this->SetId(parsed);
//			break;
//		case 1:
//			this->SetCmd(parsed);
//			break;
//		case 2:
//			this->SetParams(parsed);
//			break;
//		case 3:
//			if(!this->VerifyCRC(atol(parsed))){
//				isInit = false;
//				return;
//			}
//			break;
//		}
//		paramNum++;
//		parsed = strtok(NULL,"<->");
//	}
	isInit = true;
}

ComMessage::ComMessage(int id,char* cmd,char* params) {
	if ((params == NULL) || (cmd == NULL)){
		isInit = false;
		return;
	}
	Id = id;
//	CmdHash = this->hash((unsigned char*)cmd);
	int len = strlen(cmd)+1;
	Serial.println(len);
//	Cmd = (char *)malloc(len);
//	strncpy(Cmd,cmd,len);
	sprintf (Cmd, "%s", cmd);
	len = strlen(params)+1;
	Serial.println(len);
//	Params = (char *)malloc(len);
//	strncpy(Params,params,len);
	sprintf(Params, "%s", params);
	Serial.print("Id:");
	Serial.println(Id);
	Serial.print("cmd:");
	Serial.println(cmd);
	Serial.print("Cmd:");
	Serial.println(Cmd);
	Serial.print("params:");
	Serial.println(params);
	Serial.print("Params:");
	Serial.println(Params);
}

ComMessage::~ComMessage() {
//	if(Cmd!=NULL)
//		free (Cmd);
//	if(Params!=NULL)
//		free (Params);
}
//
//void ComMessage::SetCmd(char* cmd)
//{
//	CmdHash = this->hash((unsigned char*)cmd);
//	Cmd = (char *)malloc(strlen(cmd)+1);
//	strcpy(Cmd,cmd);
//}
//
//void ComMessage::SetParams(char* params)
//{
//	Params = (char *)malloc(strlen(params)+1);
//	strcpy(Params,params);
//}
//
//void ComMessage::SetId(char* id)
//{
//	for(int i=0;id[i]!=0;i++)
//	{
//		if(!isDigit(id[i]))
//			return;
//	}
//	Id = atoi(id);
//}
//
//bool ComMessage::VerifyCRC(unsigned long crc)
//{
//	//<1234>-command-param,param-<crc>
//	this->CreateMessage();
//	unsigned long h = this->hash((unsigned char*)command);
//	if(h==crc)
//		return true;
//	return false;
//}

const char* ComMessage::SerialMessage()
{
//	char fmt[80];
	char cmd[80];
////	this->CreateMessage();
	unsigned long h = this->hash((unsigned char*)command);
//	strcpy(&command[strlen(command)+1],"-<%n>");
////	sprintf(cmd, "%s-<%lu>", command, h);
//	sprintf(dest,fmt, command,h);
////	strcpy(command, cmd);
//	sprintf(command,"<%d>-%s-%s",Id,Cmd,Params);
	Serial.print("Id:");
	Serial.println(Id);
	Serial.print("Cmd:");
	Serial.println(Cmd);
	Serial.print("Params:");
	Serial.println(Params);

	Serial.println(command);
	Serial.println(h);
	return command;
}

const char* ComMessage::SerialMessageReply()
{
	this->CreateMessageReply();
	unsigned long h = this->hash((unsigned char*)command);
	strcpy(&command[strlen(command)+1],"-<%n>");
	sprintf(command,command,h);
	return command;
}

void ComMessage::CreateMessage()
{
	sprintf(command,"<%d>-%s-%s",Id,Cmd,Params);
}

void ComMessage::CreateMessageReply()
{
	sprintf(command,"<%d>-%sReply-%s",Id,Cmd,Params);
}

//sdbm hash function. Non crypto, non crc but cpu cheap
unsigned long ComMessage::hash(unsigned char* key)
{
    unsigned long h = 0;
	while(*key) h=*key++ + (h<<6) + (h<<16) - h;
	return h;
}

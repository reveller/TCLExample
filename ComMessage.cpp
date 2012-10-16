/*
 * ComMessage.cpp
 *
 *  Created on: Oct 3, 2012
 *      Author: Zephram
 */

#include "ComMessage.h"

ComMessage::ComMessage(char * input){

	if(input[0]!="<")
		throw 1;

	Cmd = NULL;
	Params = NULL;
	Id = 0;
	CmdHash = 0;

	char* parsed = strtok(input,"<->");
	parsed = strtok(NULL,"<->");

	int paramNum = 0;

	while (parsed)
	{
		switch(paramNum)
		{
		case 0:
			this->SetId(parsed);
			break;
		case 1:
			this->SetCmd(parsed);
			break;
		case 2:
			this->SetParams(parsed);
			break;
		case 3:
			if(!this->VerifyCRC(atol(parsed)))
				throw 2;
			break;
		}
		paramNum++;
		parsed = strtok(NULL,"<->");
	}
}
ComMessage::ComMessage(int id,char* cmd,char* params) {
	// TODO Auto-generated constructor stub
	Id = id;
	CmdHash = this->hash((unsigned char*)cmd);
	Cmd = malloc(strlen(cmd)+1);
	strcpy(Cmd,cmd);
	Params = malloc(strlen(params)+1);
	strcpy(Params,params);
}

ComMessage::~ComMessage() {
	if(Cmd!=NULL)
		free Function;
	if(Params!=NULL)
		free Params;
}

void ComMessage::SetCmd(char* cmd)
{
	CmdHash = this->hash((unsigned char*)cmd);
	Cmd = malloc(strlen(cmd)+1);
	strcpy(Cmd,cmd);
}

void ComMessage::SetParams(char* params)
{
	Params = malloc(strlen(params)+1);
	strcpy(Params,params);
}

void ComMessage::SetId(char* id)
{
	for(int i=0;id[i]!=0;i++)
	{
		if(!isDigit(id[i]))
			return;
	}
	Id = atoi(id);
}

bool ComMessage::VerifyCRC(long crc)
{
	//<1234>-command-param,param-<crc>
	this->CreateMessage();
	long h = this->hash((unsigned char*)command);
	if(h==crc)
		return true;
	return false;
}

const char* ComMessage::SerialMessage()
{
	this->CreateMessage();
	long h = this->hash((unsigned char*)command);
	strcpy(&command[strlen(command)+1],"-<%n>");
	sprintf(command,command,h);
	return command;
}
const char* ComMessage::SerialMessageReply()
{
	this->CreateMessage();
	long h = this->hash((unsigned char*)command);
	strcpy(&command[strlen(command)+1],"-<%n>");
	sprintf(command,command,h);
	return command;
}

void ComMessage::CreateMessage()
{
	sprintf(command,"<%n>-%s-%s",Id,Cmd,Params);
}

void ComMessage::CreateMessageReply()
{
	sprintf(command,"<%n>-%sReply-%s",Id,Cmd,Params);
}

//sdbm hash function. Non crypto, non crc but cpu cheap
long ComMessage::hash(unsigned char* key)
{
    unsigned long h = 0;
	while(*key) h=*key++ + (h<<6) + (h<<16) - h;
	return h;
}

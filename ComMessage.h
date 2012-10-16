/*
 * ComMessage.h
 *
 *  Created on: Oct 3, 2012
 *      Author: Zephram
 */

#ifndef COMMESSAGE_H_
#define COMMESSAGE_H_
#include "arduino.h"


class ComMessage{
public:
	ComMessage(char * input);
//	ComMessage(int,char*,char*);
	~ComMessage();

//	bool VerifyCRC(unsigned long);
//	const char* SerialMessage();
//	const char* SerialMessageReply();
	int Id;
	char* Cmd;
	unsigned long  CmdHash;
	char* Params;
	bool isInit;
private:
//	unsigned long hash(unsigned char*);
//	void CreateMessage(void);
//	void CreateMessageReply(void);
	char command[100];
//	void SetId(char*);
//	void SetCmd(char*);
//	void SetParams(char*);
};

#endif /* COMMESSAGE_H_ */

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
	ComMessage(int,char*,char*);
	~ComMessage();

	bool VerifyCRC(long);
	const char* SerialMessage();
	const char* SerialMessageReply();
	int Id;
	char* Cmd;
	long  CmdHash;
	char* Params;
private:
	long hash(unsigned char*);
	void CreateMessage(void);
	void CreateMessageReply(void);
	char command[100];
	void SetId(char*);
	void SetCmd(char*);
	void SetParams(char*);
};

#endif /* COMMESSAGE_H_ */
